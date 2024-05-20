#ifndef H_TEA
#define H_TEA

#include "../pack.h"

namespace crypt
{
	//key sz = 16
	class TEA
	{
		static const uint32_t d = 0x9E3779B9;
	public:
		static const uint_fast8_t block_size = 16;

		class Enc
		{
			uint32_t key[4];
		public:
			static const uint_fast8_t block_size = 16;

			Enc(const uint8_t* k)
			{
				conv::pack<4, endianness::BIG_ENDIAN>(k, 16, key);
			}

			void process(uint8_t* r) const
			{
				uint32_t t[2];
				conv::pack<4, endianness::BIG_ENDIAN>(r, 8, t);
				uint32_t s = 0;
				for(uint8_t i = 0; i < 32; i++)
				{
					s += d;
					t[0] += ((t[1]<<4) + key[0]) ^ (t[1] + s) ^ ((t[1]>>5) + key[1]);
					t[1] += ((t[0]<<4) + key[2]) ^ (t[0] + s) ^ ((t[0]>>5) + key[3]);
				}
				conv::unpack<4, endianness::BIG_ENDIAN>(t, 2, r);
			}
		};

		class Dec
		{
			uint32_t key[4];
		public:
			static const uint_fast8_t block_size = 16;

			Dec(const uint8_t* k)
			{
				conv::pack<4, endianness::BIG_ENDIAN>(k, 16, key);
			}

			void process(uint8_t* r) const
			{
				uint32_t t[2];
				conv::pack<4, endianness::BIG_ENDIAN>(r, 8, t);
				uint32_t s = d * 32;
				for(uint8_t i = 0; i < 32; i++)
				{
					t[1] -= ((t[0]<<4) + key[2]) ^ (t[0] + s) ^ ((t[0]>>5) + key[3]);
					t[0] -= ((t[1]<<4) + key[0]) ^ (t[1] + s) ^ ((t[1]>>5) + key[1]);
					s -= d;
				}
				conv::unpack<4, endianness::BIG_ENDIAN>(t, 2, r);
			}
		};
	};
}

#endif
