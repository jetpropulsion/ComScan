#pragma once

#include <cstdint>

#define MAX_CHARS_PER_1_UNSIGNED_BYTE			3								//255
#define MAX_CHARS_PER_2_UNSIGNED_BYTES		5								//65,535
#define MAX_CHARS_PER_3_UNSIGNED_BYTES		8								//16,777,215
#define MAX_CHARS_PER_4_UNSIGNED_BYTES		10							//4,294,967,295
#define MAX_CHARS_PER_5_UNSIGNED_BYTES		13							//1,099,511,627,775
#define MAX_CHARS_PER_6_UNSIGNED_BYTES		15							//281,474,976,710,655
#define MAX_CHARS_PER_7_UNSIGNED_BYTES		17							//72,057,594,037,927,935
#define MAX_CHARS_PER_8_UNSIGNED_BYTES		20							//18,446,744,073,709,551,615

#define MAX_CHARS_PER_1_SIGNED_BYTE				4								//-127
#define MAX_CHARS_PER_2_SIGNED_BYTES			6								//-32,767
#define MAX_CHARS_PER_3_SIGNED_BYTES			8								//-8,388,607
#define MAX_CHARS_PER_4_SIGNED_BYTES			11							//-2,147,483,647
#define MAX_CHARS_PER_5_SIGNED_BYTES			13							//-549,755,813,887
#define MAX_CHARS_PER_6_SIGNED_BYTES			16							//-140,737,488,355,327
#define MAX_CHARS_PER_7_SIGNED_BYTES			18							//-36,028,797,018,963,967
#define MAX_CHARS_PER_8_SIGNED_BYTES			20							//-9,223,372,036,854,775,807

#define MAX_CHARS_PER_1_BYTE							MAX_CHARS_PER_1_SIGNED_BYTE
#define MAX_CHARS_PER_2_BYTES							MAX_CHARS_PER_2_SIGNED_BYTES
#define MAX_CHARS_PER_3_BYTES							MAX_CHARS_PER_3_SIGNED_BYTES
#define MAX_CHARS_PER_4_BYTES							MAX_CHARS_PER_4_SIGNED_BYTES
#define MAX_CHARS_PER_5_BYTES							MAX_CHARS_PER_5_SIGNED_BYTES
#define MAX_CHARS_PER_6_BYTES							MAX_CHARS_PER_6_SIGNED_BYTES
#define MAX_CHARS_PER_7_BYTES							MAX_CHARS_PER_7_SIGNED_BYTES
#define MAX_CHARS_PER_8_BYTES							MAX_CHARS_PER_8_SIGNED_BYTES

#define MAX_CHARS_PER_SID									1+1+(MAX_CHARS_PER_1_BYTE+1)*2+(MAX_CHARS_PER_6_BYTES+1)+(MAX_CHARS_PER_4_BYTES+1)*SID_MAX_SUB_AUTHORITIES+1
#define MAX_CHARS_PER_GUID								1+(8+1)+((4+1)*3)+(12)+1
/*
#define MAX_CHARS_PER_GUID								1 +\ 						//	'{'								
																					8 +\ 						//	"01234567"				
																					1 +\ 						//	'-'								
																					4 +\ 						//	"0123"						
																					1 +\ 						//	'-'								
																					4 +\ 						//	"0123"						
																					1 +\ 						//	'-'								
																					4 +\ 						//	"0123"						
																					1 +\ 						//	'-'								
																					12 +\						//	"012345678901"		
																					1								//	'}'								
*/

#define MAX_GUID_STRING_LENGTH_W					( MAX_CHARS_PER_GUID * sizeof(wchar_t) )
#define MAX_GUID_STRING_LENGTH_A					( MAX_CHARS_PER_GUID * sizeof(char) )
#define MAX_GUID_STRING_SIZE_W						( (MAX_CHARS_PER_GUID + 1) * sizeof(wchar_t) )
#define MAX_GUID_STRING_SIZE_A						( (MAX_CHARS_PER_GUID + 1) * sizeof(char) )

#if( defined(UNICODE) )
	#define MAX_GUID_STRING_LEN							MAX_GUID_STRING_LENGTH_W
	#define MAX_GUID_STRING_SIZE						MAX_GUID_STRING_SIZE_W
#else
	#define MAX_GUID_STRING_LEN							MAX_GUID_STRING_LENGTH_A
	#define MAX_GUID_STRING_SIZE						MAX_GUID_STRING_SIZE_A
#endif

#define KiB					(* 1024)
#define MiB					(KiB * 1024)
#define GiB					(MiB * 1024)
#define TiB					(GiB * 1024)

#define KB					(* 1000)
#define MB					(KB * 1000)
#define GB					(MB * 1000)
#define TB					(GB * 1000)

