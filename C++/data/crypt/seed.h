#ifndef H_SEED
#define H_SEED

#include "../../math/base/math_.h"
#include "../pack.h"

namespace crypt
{
	//key sz = 16
	class SEED
	{
		static const uint8_t S1[256];
		static const uint8_t S2[256];

		static uint32_t G(uint32_t x)
		{
			uint32_t res = (0x01010101 * S1[x & 0xff]) & 0x3fcff3fc;
			res ^= (0x01010101 * S2[(x >> 8) & 0xff]) & 0xfc3fcff3;
			res ^= (0x01010101 * S1[(x >> 16) & 0xff]) & 0xf3fc3fcf;
			res ^= (0x01010101 * S2[(x >> 24) & 0xff]) & 0xcff3fc3f;
			return res;
		}

		static void Init(const uint8_t* k, uint64_t* key)
		{
			uint64_t t[2];
			conv::pack<8, endianness::BIG_ENDIAN>(k, 16, t);

			static const uint32_t KC[16] = {
				0x9e3779b9, 0x3c6ef373, 0x78dde6e6, 0xf1bbcdcc, 0xe3779b99, 0xc6ef3733, 0x8dde6e67, 0x1bbcdccf,
				0x3779b99e, 0x6ef3733c, 0xdde6e678, 0xbbcdccf1, 0x779b99e3, 0xef3733c6, 0xde6e678d, 0xbcdccf1b
			};
			for(uint_fast8_t i = 0; i < 16; i++)
			{
				key[i] = G((t[0]>>32) + (t[1]>>32) - KC[i]);
				key[i] = (key[i]<<32) | G(static_cast<uint32_t>(t[0]) - static_cast<uint32_t>(t[1]) + KC[i]);
				if((i & 1) == 0)
				{
					t[0] = rotr(t[0], 8);
				}
				else
				{
					t[1] = rotl(t[1], 8);
				}
			}
		}

		static void Process(const uint64_t* key, uint8_t* r)
		{
			uint32_t t[4];
			conv::pack<4, endianness::BIG_ENDIAN>(r, 16, t);
			for(uint_fast8_t i = 0;;i++)
			{
				uint32_t f0 = t[2] ^ (key[i] >> 32);
				uint32_t f1 = t[3] ^ static_cast<uint32_t>(key[i]);
				f1 = G(f0 ^ f1);
				f0 = G(f0 + f1);
				f1 = G(f0 + f1);
				t[0] ^= f0 + f1;
				t[1] ^= f1;
				if(i == 15)
					break;
				std::swap(t[0], t[2]);
				std::swap(t[1], t[3]);
			}
			conv::unpack<4, endianness::BIG_ENDIAN>(t, 4, r);
		}
	public:
		static const uint_fast8_t block_size = 16;

		class Enc
		{
			uint64_t key[16];
		public:
			static const uint_fast8_t block_size = 16;

			Enc(const uint8_t* k)
			{
				Init(k, key);
			}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};

		class Dec
		{
			uint64_t key[16];
		public:
			static const uint_fast8_t block_size = 16;

			Dec(const uint8_t* k)
			{
				Init(k, key);
				std::reverse(key, key + 16);
			}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};
	};
}

#endif
