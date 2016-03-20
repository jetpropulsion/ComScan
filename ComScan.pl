#!/c/perl64/perl.exe
use warnings qw(all);
use strict;
use integer;
no bigint;
use utf8;
use Fcntl;
use diagnostics;
use Win32;
use Digest::CRC;

##########################################################################################################
#### Configurable Parameters - START
##########################################################################################################

#### NOTE: Please change these file regex and include dirs to best match your environment


### File regular expressions: Search in all files having extensions provided in braces
my $file_regex = qr/^[^\.]+\.(?:h|hpp|hh|inl|c|cpp|cc|cxx)$/io;


### Well Known and Documented locations of Microsoft header files
my @include_dirs =
(
	'%ProgramFiles(x86)%\Microsoft SDKs\Windows\v7.1A',
	'%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC'
);

##########################################################################################################
#### Configurable Parameters - END
##########################################################################################################

### UUID storage hashes
my %com_tree;
my %guid_types;
my %memory_tree;

### Forward declarations
sub recurse($);
sub process_includes($);

##########################################################################################################
#### Main file processing function - definition
##########################################################################################################

sub process_includes($)
{
	my $rarr = \@{(shift)};
	### Include directory loop
	for(my $i = 0; $i < scalar(@$rarr); ++$i)
	{
		my $dir = Win32::ExpandEnvironmentStrings($rarr->[$i]);
		print STDERR "INFO: $dir\n";
		if(!defined($dir) || (!-d $dir))
		{
			print STDERR "ERROR: Directory \"$dir\" does not exist\n";
			next;
		}
		$dir = substr($dir, 0, length($dir)-1) if(substr($dir, length($dir)-1, 1) eq '\\');
		recurse($dir);
	}

	### Sanity check, wheter we harvested a single UUID
	my $total_identifiers = scalar(keys(%com_tree));
	printf STDERR "\n### Finished: Total Identifiers: %u\n", $total_identifiers;
	if($total_identifiers == 0)
	{
		print STDERR "WARNING: Not a single UU identifier has been harvested, double-check the include file locations\n";
		exit;
	}

	#### Display statistics about interface definition sources and macros
	foreach my $guid(keys %com_tree)
	{
		foreach my $guid_key(@{$com_tree{$guid}})
		{
			++$guid_types{$guid_key->{type}};
		}
	}
	foreach my $guid_type(sort { $a cmp $b } keys %guid_types)
	{
		printf STDERR "### Type %s occured %u times\n", $guid_type, $guid_types{$guid_type};
	}

	### Convert storage definition format to raw memory UUID format (yes, it looks different in memory)
	foreach my $guid(sort keys %com_tree)
	{
		my $memory_guid = guid_to_memory_guid($guid);
		@{$memory_tree{$memory_guid}} = @{$com_tree{$guid}};
	}
}

##########################################################################################################
#### Constants
##########################################################################################################
my $guid_space=qr/[\s\r\n]*/o;
my $guid_long = qr/[\s\r\n]*(?:0x)?([[:xdigit:]]{1,8})(?:[l|L]?)[\s\r\n]*/o;
my $guid_short = qr/[\s\r\n]*(?:0x)?([[:xdigit:]]{1,4})[\s\r\n]*/o;
my $guid_byte = qr/[\s\r\n]*(?:0x)?([[:xdigit:]]{1,2})[\s\r\n]*/o;
my $raw_guid_long = qr/([[:xdigit:]]{8})/o;
my $raw_guid_short = qr/([[:xdigit:]]{4})/o;
my $raw_guid_byte = qr/([[:xdigit:]]{2})/o;
my $raw_guid_twelve=qr/$raw_guid_byte$raw_guid_byte$raw_guid_byte$raw_guid_byte$raw_guid_byte$raw_guid_byte/o;
##########################################################################################################

# Function parses string UUID and stores it as binary data (specialization for 11 matches)
sub parse_guid1(@)
{
	my $parts = scalar(@_);
	die "### Error, expected 11 chunks insted of $parts provided" unless($parts == 11);

	my $guid = sprintf
	(
		'%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x',
		unpack('N', pack('H8', substr("0" x 8 . $_[0], -8))),
		unpack('n', pack('H4', substr("0" x 4 . $_[1], -4))),
		unpack('n', pack('H4', substr("0" x 4 . $_[2], -4))),
		unpack('C', pack('H2', substr("0" x 2 . $_[3], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[4], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[5], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[6], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[7], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[8], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[9], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[10], -2)))
	);

	return $guid;
}

# Function parses string UUID and stores it as binary data (specialization for 10 matches)
sub parse_guid2(@)
{
	my $parts = scalar(@_);
	die "### Error, expected 10 chunks insted of $parts provided" unless($parts == 10);

	my $guid = sprintf
	(
		'%08x%04x%04x%04x%02x%02x%02x%02x%02x%02x',
		unpack('N', pack('H8', substr("0" x 8 . $_[0], -8))),
		unpack('n', pack('H4', substr("0" x 4 . $_[1], -4))),
		unpack('n', pack('H4', substr("0" x 4 . $_[2], -4))),
		unpack('n', pack('H4', substr("0" x 4 . $_[3], -4))),
		unpack('C', pack('H2', substr("0" x 2 . $_[4], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[5], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[6], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[7], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[8], -2))),
		unpack('C', pack('H2', substr("0" x 2 . $_[9], -2)))
	);

	return $guid;
}


# C++ in-memory class builder, lets put together all relevant information
sub store_guid($$$$$)
{
	my $guid = $_[0];
	my $name = $_[1];
	my $type = $_[2];
	my $file = $_[3];
	my $args = $_[4];
	my %parts = ( name=>$name, type=>$type, file=>$file, args=>$args );

	if(exists($com_tree{$guid}))
	{
		foreach my $leaf(@{$com_tree{$guid}})
		{
			if($leaf->{file} eq $file)
			{
				# don't store dupicate UUID's from same file
				return;
			}
		}
	}

	push(@{$com_tree{$guid}}, \%parts);
}

#	Hash key is compiled regex, if matching of input is successfull against it, call the lambda-like function stored in hash value.
my %processors =
(
	qr/MIDL_INTERFACE\($guid_space\"$raw_guid_long\-$raw_guid_short\-$raw_guid_short\-$raw_guid_short\-$raw_guid_twelve\"$guid_space\)\s*[\r\n]+\s*([[:alpha:]][[:alnum:]\_]+)/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = pop(@_);
		store_guid(parse_guid2(@_), $name, 'MIDL_INTERFACE', $file, join(',', @_));
	},

	qr/DEFINE_GUID\($guid_space([^\s\r\n,]+),$guid_long,$guid_short,$guid_short,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte\)/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'DEFINE_GUID', $file, join(',', @_));
	},

	qr/DECLSPEC_UUID\($guid_space\"$raw_guid_long\-$raw_guid_short\-$raw_guid_short-$raw_guid_short-$raw_guid_twelve\"$guid_space\)$guid_space([^\;\s\r\n]+)$guid_space\;/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = pop(@_);
		store_guid(parse_guid2(@_), $name, 'DECLSPEC_UUID', $file, join(',', @_));
	},

	qr/EXTERN_GUID\($guid_space([^\s\r\n,]+)$guid_space,$guid_long,$guid_short,$guid_short,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'EXTERN_GUID', $file, join(',', @_));
	},

	qr/GUID_BUILDER\($guid_space([^\s\r\n,]+)$guid_space,$guid_space$raw_guid_long$guid_space,$guid_space$raw_guid_short$guid_space,$guid_space$raw_guid_short$guid_space,$guid_space$raw_guid_byte$guid_space,$guid_space$raw_guid_byte$guid_space,$guid_space$raw_guid_byte$guid_space,$guid_space$raw_guid_byte$guid_space,$guid_space$raw_guid_byte$guid_space,$guid_space$raw_guid_byte$guid_space,$guid_space$raw_guid_byte$guid_space,$guid_space$raw_guid_byte$guid_space\)/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'GUID_BUILDER', $file, join(',', @_));
	},

	qr/const[\s\r\n]+GUID[\s\r\n]+([^\s\r\n\=]+)$guid_space\=$guid_space\{$guid_long,$guid_short,$guid_short,$guid_space\{$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte\}$guid_space\}$guid_space\;/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'GUID', $file, join(',', @_));
	},

	qr/const[\s\r\n]+IID[\s\r\n]+([^\s\r\n\=]+)$guid_space\=$guid_space\{$guid_long,$guid_short,$guid_short,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte\}$guid_space\}$guid_space\;/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'IID', $file, join(',', @_));
	},

	qr/const[\s\r\n]+CLSID[\s\r\n]+([^\s\r\n\=]+)$guid_space\=$guid_space\{$guid_long,$guid_short,$guid_short,$guid_space\{$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte\}$guid_space\}$guid_space\;/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'CLSID', $file, join(',', @_));
	},

	qr/const[\s\r\n]+GUID[\s\r\n]+OLEDBDECLSPEC[\s\r\n]+([^\s\r\n\=]+)$guid_space\=$guid_space\{$guid_long,$guid_short,$guid_short,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte\}/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'GUID OLEDBDECLSPEC', $file, join(',', @_));
	},
	
	qr/const[\s\r\n]+DBID[\s\r\n]+OLEDBDECLSPEC[\s\r\n]+([^\s\r\n\=]+)$guid_space\=$guid_space\{$guid_long,$guid_short,$guid_short,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte,$guid_byte\}/o
	=>
	sub(@)
	{
		my $file = pop(@_);
		my $name = shift(@_);
		store_guid(parse_guid1(@_), $name, 'DBID OLEDBDECLSPEC', $file, join(',', @_));
	},
);

##########################################################################################################

# This function permutes binary data parsed from definition to resemble the same UUID layout in memory
sub guid_to_memory_guid(@)
{
	my @guid = unpack('C*', pack('H*', @_));
	my $guid_length = scalar(@guid);
	die "### Expected sixteen bytes instead of $guid_length" unless($guid_length == 16);

	my $temp;
	#swap 3rd and 0th
	$temp = $guid[ 3 ];
	$guid[ 3 ] = $guid[ 0 ];
	$guid[ 0 ] = $temp;

	#swap 2nd and 1st
	$temp = $guid[ 2 ];
	$guid[ 2 ] = $guid[ 1 ];
	$guid[ 1 ] = $temp;

	#swap 5th and 4th
	$temp = $guid[ 5 ];
	$guid[ 5 ] = $guid[ 4 ];
	$guid[ 4 ] = $temp;

	#swap 7th and 6th
	$temp = $guid[ 7 ];
	$guid[ 7 ] = $guid[ 6 ];
	$guid[ 6 ] = $temp;

	return unpack('H*', pack('C*', @guid));
}

##########################################################################################################

# Function treats any provided file as binary file and performs search
# PROS: Multiline complex constructs can be matched, faster
# CONS: No possiblity of line number tracking, but we don't need it for this purpose at all. Send your comments.

sub binary_regex_search($)
{
	my ($file, $hfile) = shift;
	unless(sysopen($hfile, $file, O_RDONLY | O_BINARY))
	{
		print STDERR "*** skipping file \"$file\", can't open it because $!\n";
		return;
	}

	my ($buffer, $temp, $read);
	while(($read = sysread($hfile, $temp, 65536)) != 0)
	{
		$buffer.=$temp;
	}
	close($hfile);

	my ($processing_file);
	my ($out);
	my @processor_regex = keys(%processors);
	for(my $i = 0; $i < scalar(@processor_regex); ++$i)
	{
		my $line_regex = $processor_regex[$i];
		my $print_fn = $processors{$line_regex};

		my $cols;
		$line_regex =~ s/([^\\]\([^\?])/++$cols, $1/ge;
		$cols = 1 unless($cols);

		my (@matches);
		next unless((@matches) = ($buffer =~ /$line_regex/g));

		unless($out++)
		{
			#$processing_file = substr($file, length($dir) + 1);
			$processing_file = $file;
			print STDERR "# Processing \"$processing_file\"\n";
		}
		while(scalar(@matches) != 0)
		{
			my @args = splice(@matches, 0, $cols);
			push(@args, $processing_file);
			&{$processors{$line_regex}}(@args);
		}
	}
}

##########################################################################################################

# Function traverses provided directory, if a file is directory, recurses, if file is non-directory, performs search
sub recurse($)
{
	my ($dir, $hdir) = shift;
	opendir($hdir, $dir) || die "Can't open dir \"$dir\" because $!\n";
	my (@files) = readdir($hdir);
	closedir($hdir);

	foreach(sort { uc($a) cmp uc($b) } @files)
	{
		my ($file)="$dir\\$_";

		if(-d $file)
		{
			next if(/^\.{1,2}$/);
			recurse($file);
			next;
		}

		next if(defined($file_regex) && ($_ !~ /$file_regex/));

		my @attr = stat($file);
		my $size = $attr[7];
		if($size == 0)
		{
			print STDERR "WARNING: skipping file \"$file\", file is zero bytes long\n";
			next;
		}
		binary_regex_search($file);
	}
}

###########################################################################################################################################

# Following function is used to generate C++ UUID definition structures
sub dump_references($)
{
	my $rhash = \%{(shift)};
	my @memory_references;
	foreach my $guid(sort { $a cmp $b } keys %{$rhash})
	{
		my $display_guid = $guid;
		my @guid_array;
		$display_guid =~ s/([[:xdigit:]]{2})/\&{ push(@guid_array, "0x$1"); }/ge;
		$display_guid = join(',', @guid_array);
		my $alias_count = scalar @{$rhash->{$guid}};
		my $guid_symbol = sprintf('guid_%s', $guid);

		printf STDOUT ("static GUID_DEFINITION const %s = \n{\n", $guid_symbol);
		printf STDOUT ("\t{\n\t\t%s\n\t},", $display_guid);


		#	C++ static CRC32
		#printf STDOUT ("\n\tJetLib::Static::crc32_ansi(%s.guid),", $guid_symbol);

		#	Perl Digest::CRC generated CRC32
		my $crc32 = Digest::CRC->new(type=>"crc32", xorout=>0xffffffff, poly=>0xedb88320, init=>0xffffffff, width=>32, revin=>1, revout=>1);
		$crc32->add(pack('H*', $guid));
		printf STDOUT ("\n\t0x%s,", $crc32->hexdigest);


		#	Number of GUID_DEFINITION structures times $alias_count times follows
		printf STDOUT ("\n\t%u,", $alias_count);
		printf STDOUT "\n\t{\n";

		foreach my $guid_key( sort { $a->{name} cmp $b->{name} || $a->{type} cmp $b->{type} } @{$rhash->{$guid}} )
		{
			my ($escaped_file) = ($guid_key->{file});
			$escaped_file =~ s/\\/\\\\/g;
			printf STDOUT "\t\tL\"%s\", L\"%s\", L\"%s\",\n", $escaped_file, $guid_key->{type}, $guid_key->{name};
		}
		print STDOUT "\t}\n};\n";

		push(@memory_references, $guid);
	};

	my $memory_references_count = scalar(@memory_references);
	printf STDOUT "static GUID_DEFINITION const *guid_definitions[%u] =\n{\n", $memory_references_count;
	for(my $i = 0; $i < $memory_references_count; ++$i)
	{
		my $guid = $memory_references[$i];
		my $mref = \%{ @{$rhash->{$guid}}[0] };
		my $name = '<unknown>';
		if(exists($mref->{name}))
		{
			$name =  $mref->{name};
			$name = '<empty>' if($name =~ /^\s*$/o);
		}
		printf STDOUT "\t\&guid_%s,\t\t\/\/%s\n", $guid, $name;
	}
	print STDOUT "};\n";

}

###########################################################################################################################################
### Main parsing code invocation
###########################################################################################################################################

process_includes(\@include_dirs);

###########################################################################################################################################
### Template
###########################################################################################################################################

# Following textual template is used to generate C++ UUID definition structures

print STDOUT << 'EOF';
#pragma once

#include "CRC32.h"

/******************************************************************************
********************************************************************************
***
*** This is autogenerated C++ header file, please DO NOT edit it manually,
*** instead, change the Perl code genrating this file.
***
********************************************************************************
 ******************************************************************************/

typedef struct GUID_INFO
{
	wchar_t const * const header_file;
	wchar_t const * const header_macro;
	wchar_t const * const header_name;
}
GUID_INFO;

typedef struct GUID_DEFINITION
{
	uint8_t const guid[16];
	uint32_t const guid_crc32;
	uint32_t guid_info_count;
#pragma warning(push)
#pragma warning(disable: 4200)											//warning C4200: nonstandard extension used: zero-sized array in struct/union
	GUID_INFO const guid_info[];
#pragma warning(pop)
}
GUID_DEFINITION;

EOF
###########################################################################################################################################
###########################################################################################################################################
###########################################################################################################################################

# In the middle of the constant text, output const initializers (someone said "server parsed scripts"?)
dump_references(\%memory_tree);

###########################################################################################################################################
###########################################################################################################################################
###########################################################################################################################################
print STDOUT << 'EOF';

///////////////////////////////////////////////////////////////////////////////////////////

bool FindGuidLinear(GUID const& guid_to_find, GUID_DEFINITION const *&guid_definition)
{
	for (uint32_t i = 0; i < lengthof(guid_definitions); ++i)
	{
		GUID_DEFINITION const* gd = guid_definitions[i];
		if (!::memcmp(&guid_to_find, &gd->guid, sizeof(GUID)))
		{
			guid_definition = gd;
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////

static __declspec(noinline) int __cdecl FindGuidBinaryCompare(void *context, GUID const** key, GUID_DEFINITION const** current)
{
	UNREFERENCED_PARAMETER(context);
	return ::memcmp(key, &(*current)->guid, sizeof(GUID));
}
bool FindGuidBinary(GUID const& guid_to_find, GUID_DEFINITION const *&guid_definition)
{
	GUID_DEFINITION const **result = (GUID_DEFINITION const **)::bsearch_s(&guid_to_find, guid_definitions, lengthof(guid_definitions), sizeof(GUID_DEFINITION*), (int(*)(void*, void const *, void const *))FindGuidBinaryCompare, nullptr);
	if (result == nullptr)
	{
		return false;
	}
	guid_definition = *result;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////

#include "Format.h"
void DumpGuidDefinition(GUID_DEFINITION const& guid_definition, std::wostream& output)
{
	output	<< L"GUID { "
					<< JETLIB_UINT32_FORMATW << *((uint32_t*)&guid_definition.guid[0]) << L", "
					<< JETLIB_UINT16_FORMATW << *((uint16_t*)&guid_definition.guid[4]) << L", "
					<< JETLIB_UINT16_FORMATW << *((uint16_t*)&guid_definition.guid[6]) << L", { "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[8] << L", "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[9] << L", "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[10] << L", "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[11] << L", "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[12] << L", "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[13] << L", "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[14] << L", "
					<< JETLIB_UINT8_FORMATW << guid_definition.guid[15] << L" } }\t\t//CRC32="
					<< JETLIB_UINT32_FORMATW << guid_definition.guid_crc32
					<< std::endl;


	for (uint32_t i = 0; i < guid_definition.guid_info_count; ++i)
	{
		output << JETLIB_INT32_FORMATW << i
							<< L": header=\"" << guid_definition.guid_info[i].header_file
							<< L"\", macro=\"" << guid_definition.guid_info[i].header_macro
							<< L"\", name=\"" << guid_definition.guid_info[i].header_name
							<< L"\"" << std::endl;
	}

	output << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////////////

void DumpGuidDefinitions(std::wostream& output)
{
	for (std::size_t i = 0; i < lengthof(guid_definitions); ++i)
	{
		JetLib::Hash::CRC32 crc32(JetLib::Hash::CRC32::ANSI_Rev);
		crc32.Update(guid_definitions[i]->guid, sizeof(guid_definitions[i]->guid));
		uint32_t const fresh_crc32 = crc32.Get();

		if(fresh_crc32 != guid_definitions[i]->guid_crc32)
		{
			output	<< L"ERROR: Data integrity failure, CRC32 is reported to be " << JETLIB_UINT32_FORMATW << guid_definitions[i]->guid_crc32
							<< L", but in reality it is " << JETLIB_UINT32_FORMATW << fresh_crc32
							<< std::endl;
			output.flush();
			::SleepEx(1000, FALSE);
			throw std::exception(LINE_INFO);
		}

		DumpGuidDefinition(*guid_definitions[i], output);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

EOF

