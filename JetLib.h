#pragma once

#include <cassert>

//Macro LINE_INFO can provide information about file and line where possible error occured
#define LINE_SEPARATOR				", "
#define LINE_INFO							__FILE__ LINE_SEPARATOR _CRT_STRINGIZE(__LINE__)


//Macro WIN32_ASSERT(expression) provides single line Win32 API return value checking, throwing std::exception with code position when the exception occurs
#define WIN32_ASSERT(expression)\
	::SetLastError(NO_ERROR);\
	if(!(expression))\
	{\
		throw std::exception(LINE_INFO);\
	}


//Macro WIN32_ASSERT_VALUE(expression) is similar to WIN32_ASSERT, except it doesn't automatically clear last error field in TEB, useful for more complex checking
#define WIN32_ASSERT_VALUE(expression)\
	if(!(expression))\
	{\
		throw std::exception(LINE_INFO);\
	}


//Macro DYNAMIC_ASSERT(expression) throws exception when expression is false (#__VA_ARGS__ passes arguments as is, i.e. without expanding definition)
#define DYNAMIC_ASSERT(expression)\
	if(!(expression))\
	{\
		throw std::exception(LINE_INFO);\
	}


//Macro STATIC_ASSERT(expression) prevents compiling when irregular state of the expected values is not met (example: { STATIC_ASSERT(sizeof(void*) != sizeof(char)); })
#define STATIC_ASSERT(...)\
	static_assert(__VA_ARGS__, #__VA_ARGS__)


//Inline helper, returns length of array provided, similar to (sizeof(TypeArray) / sizeof(TypeArray[0]))
template<std::size_t N, class T>
constexpr inline std::size_t lengthof(T const (&TypeArray)[N])
{
	UNREFERENCED_PARAMETER(TypeArray);
	return N;
}


//Inline helper, sets all array members to zero
template<std::size_t N, class T>
inline void zeromem(T (&TypeArray)[N])
{
	std::size_t const TypeArraySize = N * sizeof(T);
	STATIC_ASSERT(TypeArraySize == sizeof(TypeArray));

	//::RtlZeroMemory(&TypeArray, TypeArraySize);			//NOTE: use RtlZeroMemory() only when you know for sure that memory blocks will be very huge (> 64KiB in 2016.)
	::memset(&TypeArray, 0, TypeArraySize);
}


template<class T>
constexpr inline void zeromem(T &Type)
{
	::memset(&Type, 0, sizeof(T));
}
