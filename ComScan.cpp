#include <Windows.h>
#include <tchar.h>
#include <uuids.h>

#include <ios>
#include <iostream>
#include <memory>

#include "JetLib.h"
#include "Metrics.h"
#include "Format.h"

///////////////////////////////////////////////////////////////////////////////////////////
//NOTE: If compiler complains that particular file "ScannedData.h" is missing
//			please generate it first using provided perl script "ComScan.pl".
///////////////////////////////////////////////////////////////////////////////////////////

#include "ScannedData.h"

///////////////////////////////////////////////////////////////////////////////////////////
//NOTE: Before executing "ComScan.pl" script, make sure that you customized two configuration settings found near the top of file:
//				"$file_regex" - Determines which file types should be scanned for possible UUID definitions
//				"@include_dirs" - Specifies all the possible source code paths where UUID definitions could be found
///////////////////////////////////////////////////////////////////////////////////////////


static char const * const hex = "0123456789ABCDEF";
std::string parse_hex(char const *text, std::size_t length)
{
	std::size_t const max_parsed_length = (sizeof(GUID) * 3) + 2;
	std::size_t const parsed_length = max(length, max_parsed_length);

	std::unique_ptr<char> result(new char[parsed_length]);
	char *p = result.get();
	std::size_t p_index = 0;

	uint8_t p_out = 0;
	bool p_has_out = false;

	for (std::size_t i = 0; i < length; ++i)
	{
		if (std::strchr("{}-,.[]\x20", text[i]) != nullptr)
		{
			//current character is part of GUID decoration used only when displating it in the user interface
			continue;
		}
		char const* const hex_index = std::strchr(hex, ::toupper(text[i]));
		if (hex_index == nullptr)
		{
			std::cout << "unrecognized hexadecimal nibble occured at position " << (i) << std::endl;
			continue;
		}
		if (p_has_out)
		{
			//odd output, shift (mix current with previous nibble) output and add current nibble
			p_out <<= 4;
			p_out |= ((uint8_t)(hex_index - hex));
			p[ p_index++ ] = p_out;
			p_has_out = false;
		}
		else
		{
			//even output, shift output and add current octet
			p_out = ((uint8_t)(hex_index - hex));
			p_has_out = true;
		}
	}
	if (p_has_out)
	{
		p_out <<= 4;
		p[ p_index++ ] = p_out;
	}

	if(p_index != sizeof(GUID))
	{
		std::cout << "provided GUID is not 16 octets long (it is " << JETLIB_INT32_FORMATA << p_index << " bytes long)" << std::endl;
		throw std::exception();
	}

	return std::string(result.get(), p_index);
}

///////////////////////////////////////////////////////////////////////////////////////////

//3 2 1 0 - 5 4 - 7 6 - 8 9 - A B C D E F
std::string permute_guid(std::string const& raw_guid)
{
	if(raw_guid.length() != sizeof(GUID))
	{
		std::cout << "provided GUID is not 16 octets long (it is " << JETLIB_INT32_FORMATA << raw_guid.length() << " bytes long)" << std::endl;
		throw std::exception();
	}

	std::string result( raw_guid );

	char temp;

	//swap 3rd and 0th
	temp = result[ 3 ];
	result[ 3 ] = result[ 0 ];
	result[ 0 ] = temp;

	//swap 2nd and 1st
	temp = result[ 2 ];
	result[ 2 ] = result[ 1 ];
	result[ 1 ] = temp;

	//swap 5th and 4th
	temp = result[ 5 ];
	result[ 5 ] = result[ 4 ];
	result[ 4 ] = temp;

	//swap 7th and 6th
	temp = result[ 7 ];
	result[ 7 ] = result[ 6 ];
	result[ 6 ] = temp;

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, TCHAR **argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	try
	{
		if (argc != 2)
		{
			std::cout << "Usage: " << argv[0] << " <guid>" << std::endl;
			throw std::exception(LINE_INFO);
		}

		std::string const user_provided_guid = permute_guid(parse_hex(argv[1], std::strlen(argv[1])));
		DYNAMIC_ASSERT(user_provided_guid.length() == sizeof(GUID));

		std::cout << "GUID parsed, searching...";
		GUID_DEFINITION const* guid_definition = nullptr;
		if (FindGuidBinary(reinterpret_cast<GUID const&>(*user_provided_guid.c_str()), guid_definition))
		{
			std::cout << " GUID is found:" << std::endl << std::endl;
			DumpGuidDefinition(*guid_definition, std::wcout);
		}
		else
		{
			std::cout << " nothing found" << std::endl;
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "EXCEPTION: " << ex.what() << std::endl;
	}

	return 0;
}

