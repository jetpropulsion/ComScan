#pragma once

#include "Metrics.h"

#include <ios>
#include <iomanip>

#define JETLIB_ALIGN_FORMAT						std::left
#define JETLIB_MAX_ALIGN_FORMAT				std::right

#define JETLIB_RESET_FORMATW					JETLIB_ALIGN_FORMAT << std::dec << std::setw(0) << std::setfill(L' ') << std::setprecision(0)
#define JETLIB_0000_FORMATW						JETLIB_ALIGN_FORMAT << std::dec << std::setw(4) << std::setfill(L'0')
#define JETLIB_000_FORMATW						JETLIB_ALIGN_FORMAT << std::dec << std::setw(3) << std::setfill(L'0')
#define JETLIB_00_FORMATW							JETLIB_ALIGN_FORMAT << std::dec << std::setw(2) << std::setfill(L'0')
#define JETLIB_INT8_FORMATW						JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw( 1 )
#define JETLIB_INT16_FORMATW					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw( 1 )
#define JETLIB_INT32_FORMATW					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw( 1 )
#define JETLIB_INT64_FORMATW					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw( 1 )
#define JETLIB_MAX_INT8_FORMATW				JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw(MAX_CHARS_PER_1_SIGNED_BYTE)
#define JETLIB_MAX_INT16_FORMATW			JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw(MAX_CHARS_PER_2_SIGNED_BYTES)
#define JETLIB_MAX_INT32_FORMATW			JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw(MAX_CHARS_PER_4_SIGNED_BYTES)
#define JETLIB_MAX_INT64_FORMATW			JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw(MAX_CHARS_PER_8_SIGNED_BYTES)
#define JETLIB_UINT8_FORMATW					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << L"0x" << std::setfill(L'0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint8_t))
#define JETLIB_UINT16_FORMATW					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << L"0x" << std::setfill(L'0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint16_t))
#define JETLIB_UINT32_FORMATW					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << L"0x" << std::setfill(L'0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint32_t))
#define JETLIB_UINT64_FORMATW					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << L"0x" << std::setfill(L'0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint64_t))
#define JETLIB_FLOAT_FORMATW					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw( 1 ) << std::setprecision(20) << std::resetiosflags(4096)
#define JETLIB_DOUBLE_FORMATW					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(L' ') << std::dec << std::setw( 1 ) << std::setprecision(20) << std::resetiosflags(4096)
#define JETLIB_PTR_FORMATW						JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << L"0x" << std::setfill(L'0') << std::nouppercase << std::hex << std::setw(2 * sizeof(PVOID))

#define JETLIB_RESET_FORMATA					JETLIB_ALIGN_FORMAT << std::dec << std::setw(0) << std::setfill(' ') << std::setprecision(0)
#define JETLIB_0000_FORMATA						JETLIB_ALIGN_FORMAT << std::dec << std::setw(4) << std::setfill('0')
#define JETLIB_000_FORMATA						JETLIB_ALIGN_FORMAT << std::dec << std::setw(3) << std::setfill('0')
#define JETLIB_00_FORMATA							JETLIB_ALIGN_FORMAT << std::dec << std::setw(2) << std::setfill('0')
#define JETLIB_INT8_FORMATA						JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( 1 )
#define JETLIB_INT16_FORMATA					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( 1 )
#define JETLIB_INT32_FORMATA					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( 1 )
#define JETLIB_INT64_FORMATA					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( 1 )
#define JETLIB_MAX_INT8_FORMATA				JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( MAX_CHARS_PER_1_SIGNED_BYTE )
#define JETLIB_MAX_INT16_FORMATA			JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( MAX_CHARS_PER_2_SIGNED_BYTES )
#define JETLIB_MAX_INT32_FORMATA			JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( MAX_CHARS_PER_4_SIGNED_BYTES )
#define JETLIB_MAX_INT64_FORMATA			JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( MAX_CHARS_PER_8_SIGNED_BYTES )
#define JETLIB_UINT8_FORMATA					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << "0x" << std::setfill('0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint8_t))
#define JETLIB_UINT16_FORMATA					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << "0x" << std::setfill('0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint16_t))
#define JETLIB_UINT32_FORMATA					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << "0x" << std::setfill('0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint32_t))
#define JETLIB_UINT64_FORMATA					JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << "0x" << std::setfill('0') << std::nouppercase << std::hex << std::setw(2 * sizeof(uint64_t))
#define JETLIB_FLOAT_FORMATA					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( 1 ) << std::setprecision(20) << std::resetiosflags(4096)
#define JETLIB_DOUBLE_FORMATA					JETLIB_ALIGN_FORMAT << std::noshowbase << std::setfill(' ') << std::dec << std::setw( 1 ) << std::setprecision(20) << std::resetiosflags(4096)
#define JETLIB_PTR_FORMATA						JETLIB_MAX_ALIGN_FORMAT << std::noshowbase << std::uppercase << "0x" << std::setfill('0') << std::nouppercase << std::hex << std::setw(2 * sizeof(PVOID))

#if(defined(_UNICODE))
#define JETLIB_RESET_FORMAT						JETLIB_RESET_FORMATW
#define JETLIB_0000_FORMAT						JETLIB_0000_FORMATW
#define JETLIB_000_FORMAT							JETLIB_000_FORMATW
#define JETLIB_00_FORMAT							JETLIB_00_FORMATW
#define JETLIB_INT8_FORMAT						JETLIB_INT8_FORMATW
#define JETLIB_INT16_FORMAT						JETLIB_INT16_FORMATW
#define JETLIB_INT32_FORMAT						JETLIB_INT32_FORMATW
#define JETLIB_INT64_FORMAT						JETLIB_INT64_FORMATW
#define JETLIB_MAX_INT8_FORMAT				JETLIB_MAX_INT8_FORMATW
#define JETLIB_MAX_INT16_FORMAT				JETLIB_MAX_INT16_FORMATW
#define JETLIB_MAX_INT32_FORMAT				JETLIB_MAX_INT32_FORMATW
#define JETLIB_MAX_INT64_FORMAT				JETLIB_MAX_INT64_FORMATW
#define JETLIB_UINT16_FORMAT					JETLIB_UINT16_FORMATW
#define JETLIB_UINT32_FORMAT					JETLIB_UINT32_FORMATW
#define JETLIB_UINT64_FORMAT					JETLIB_UINT64_FORMATW
#define JETLIB_FLOAT_FORMAT						JETLIB_FLOAT_FORMATW
#define JETLIB_DOUBLE_FORMAT					JETLIB_DOUBLE_FORMATW
#define JETLIB_PTR_FORMAT							JETLIB_PTR_FORMATW
#else
#define JETLIB_RESET_FORMAT						JETLIB_RESET_FORMATA
#define JETLIB_0000_FORMAT						JETLIB_0000_FORMATA
#define JETLIB_000_FORMAT							JETLIB_000_FORMATA
#define JETLIB_00_FORMAT							JETLIB_00_FORMATA
#define JETLIB_INT8_FORMAT						JETLIB_INT8_FORMATA
#define JETLIB_INT16_FORMAT						JETLIB_INT16_FORMATA
#define JETLIB_INT32_FORMAT						JETLIB_INT32_FORMATA
#define JETLIB_INT64_FORMAT						JETLIB_INT64_FORMATA
#define JETLIB_MAX_INT8_FORMAT				JETLIB_MAX_INT8_FORMATA
#define JETLIB_MAX_INT16_FORMAT				JETLIB_MAX_INT16_FORMATA
#define JETLIB_MAX_INT32_FORMAT				JETLIB_MAX_INT32_FORMATA
#define JETLIB_MAX_INT64_FORMAT				JETLIB_MAX_INT64_FORMATA
#define JETLIB_UINT8_FORMAT						JETLIB_UINT8_FORMATA
#define JETLIB_UINT16_FORMAT					JETLIB_UINT16_FORMATA
#define JETLIB_UINT32_FORMAT					JETLIB_UINT32_FORMATA
#define JETLIB_UINT64_FORMAT					JETLIB_UINT64_FORMATA
#define JETLIB_FLOAT_FORMAT						JETLIB_FLOAT_FORMATA
#define JETLIB_DOUBLE_FORMAT					JETLIB_DOUBLE_FORMATA
#define JETLIB_PTR_FORMAT							JETLIB_PTR_FORMATA
#endif
