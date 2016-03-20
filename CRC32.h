#pragma once

#include <cstdint>
#include "JetLib.h"

namespace JetLib
{
	namespace Hash
	{
		class CRC32
		{
		public:
			typedef enum CRC32Poly : uint32_t
			{
				ANSI = 0x04C11DB7,						//ANSI X3.66, default
				ANSI_Rev = 0xEDB88320,				//ANSI X3.66, reversed poly
				ANSI_RevRec = 0x82608EDB,			//ANSI X3.66, reversed poly and reciprocal

				CRC32_C = 0x1EDC6F41,					//CRC32-C (Castagnoli), default
				CRC32_C_Rev = 0x82F63B78,			//CRC32-C (Castagnoli), reversed poly
				CRC32_C_RevRec = 0x8F6E37A0,	//CRC32-C (Castagnoli), reversed poly and reciprocal
			}
			CRC32Poly;
		protected:
			uint32_t Table[ 256 ];
			uint32_t TablePoly;
			uint32_t Accumulator;
			bool TableInitialized;
		protected:
			virtual void Init(uint32_t Polynomial)
			{
				uint32_t remainder;
				uint8_t b = 0;
				do
				{
					remainder = b;
					for(uint32_t bit = 8; bit > 0; --bit)
					{
						uint32_t const lsb = (remainder & 1);
						remainder = (remainder >> 1);
						remainder ^= Polynomial * lsb;
					}
					this->Table[ (uint8_t)b ] = remainder;
				}
				while(++b != 0);

				this->TablePoly = Polynomial;
				this->Accumulator = 0;
				this->TableInitialized = true;
			}
		protected:
			typedef uint32_t (JetLib::Hash::CRC32::*XorFunction)(uint32_t crc32);
			virtual uint32_t XorInvert(uint32_t crc32)
			{
				return crc32 ^ static_cast<uint32_t>(-1);
			}
			XorFunction JetLib::Hash::CRC32::XorIn;
			XorFunction JetLib::Hash::CRC32::XorOut;
		public:
			//Used in initial call and/or when small data needs to be hashed, i.e. during this function call
			virtual uint32_t Update(uint8_t const *data, size_t data_size)
			{
				DYNAMIC_ASSERT(this->TableInitialized);

				return (this->Accumulator = this->Update(data, data_size, (this->*XorIn)(0)));
			}

			//More flexible version of Update(), allowing previous or algorithmic CRC32 to be provided and mixed in LFSR chain
			virtual uint32_t Update(uint8_t const *data, size_t data_size, uint32_t crc)
			{
				DYNAMIC_ASSERT(this->TableInitialized);

				for(size_t i = 0; i < data_size; i++)
				{
					crc = this->Table[data[i] ^ (crc & 0xff)] ^ (crc >> 8);
				}
				return (this->Accumulator = (this->*XorOut)(crc));
			}

			//Reset and rebuild internal lookup polynomial table
			virtual void Reset()
			{
				DYNAMIC_ASSERT(this->TableInitialized);
				this->Init(this->TablePoly);
			}

			//Reset and rebuild internal lookup polynomial table using provided and well known CRC polynomial value
			virtual void Reset(CRC32Poly const crc32_type)
			{
				this->Init(crc32_type);
			}
		private:
			//Parameterless constructor is not intended to be used by public
			explicit CRC32() :
				XorIn(&JetLib::Hash::CRC32::XorInvert),
				XorOut(&JetLib::Hash::CRC32::XorInvert),
				TableInitialized(false),
				Accumulator(0),
				TablePoly(0)
			{
			}
		public:
			CRC32(CRC32Poly const crc32_type = CRC32Poly::ANSI) :
				XorIn(&JetLib::Hash::CRC32::XorInvert),
				XorOut(&JetLib::Hash::CRC32::XorInvert),
				TableInitialized(false),
				Accumulator(0),
				TablePoly(0)
			{
				this->Init(crc32_type);
			}
			virtual ~CRC32()
			{
				zeromem(this->TablePoly);
				zeromem(this->Table);
			}
			virtual uint32_t Get() const
			{
				return this->Accumulator;
			}
		};	//END: class JetLib::Hash::CRC32 {
	};	//END: namespace JetLib::Hash {
};	//END: namespace JetLib {


