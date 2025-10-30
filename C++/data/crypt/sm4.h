#ifndef H_SM4
#define H_SM4

#include <algorithm>

#include "../../math/base/math_.h"
#include "../pack.h"

namespace crypt
{
	//key sz = 16
	class SM4
	{
		static uint32_t sb(uint32_t c)
		{
			uint8_t t[4];
			bconv<1, 4, endianness::LITTLE_ENDIAN>::unpack(c, t);
			t[0] = Sbox[t[0]];
			t[1] = Sbox[t[1]];
			t[2] = Sbox[t[2]];
			t[3] = Sbox[t[3]];
			return bconv<1, 4, endianness::LITTLE_ENDIAN>::pack(t);
		}

		static uint32_t kg(uint32_t c)
		{
			auto k = sb(c);
			return k ^ rotl(k, 13) ^ rotl(k, 23);
		}

		static uint32_t cr(uint32_t c)
		{
			auto k = sb(c);
			return k ^ rotl(k, 2) ^ rotl(k, 10) ^ rotl(k, 18) ^ rotl(k, 24);
		}

		static void Init(const uint8_t* k, uint32_t* key)
		{
			uint32_t K[4];
			conv::pack<4, endianness::BIG_ENDIAN>(k, 16, K);
			K[0] ^= 0xa3b1bac6;
			K[1] ^= 0x56aa3350;
			K[2] ^= 0x677d9197;
			K[3] ^= 0xb27022dc;

			for(uint_fast8_t i = 0; i < 32; i+=4)
			{
				K[0] ^= kg(K[1] ^ K[2] ^ K[3] ^ ke[i]);
				K[1] ^= kg(K[2] ^ K[3] ^ K[0] ^ ke[i+1]);
				K[2] ^= kg(K[3] ^ K[0] ^ K[1] ^ ke[i+2]);
				K[3] ^= kg(K[0] ^ K[1] ^ K[2] ^ ke[i+3]);
				std::copy_n(K, 4, key + i);
			}
		}

		static const uint8_t Sbox[256];
		static const uint32_t ke[32];

		static void Process(const uint32_t* key, uint8_t* r)
		{
			uint32_t e[4];
			conv::pack<4, endianness::BIG_ENDIAN>(r, 16, e);
			for(uint_fast8_t i = 0; i < 32; i+=4)
			{
				e[0] ^= cr(e[1] ^ e[2] ^ e[3] ^ key[i]);
				e[1] ^= cr(e[2] ^ e[3] ^ e[0] ^ key[i+1]);
				e[2] ^= cr(e[3] ^ e[0] ^ e[1] ^ key[i+2]);
				e[3] ^= cr(e[0] ^ e[1] ^ e[2] ^ key[i+3]);
			}
			bconv<1, 4, endianness::BIG_ENDIAN>::unpack(e[3], r);
			bconv<1, 4, endianness::BIG_ENDIAN>::unpack(e[2], r + 4);
			bconv<1, 4, endianness::BIG_ENDIAN>::unpack(e[1], r + 8);
			bconv<1, 4, endianness::BIG_ENDIAN>::unpack(e[0], r + 12);
		}

	public:
		static const uint_fast8_t block_size = 16;

		class Enc
		{
			uint32_t key[32];
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
			uint32_t key[32];
		public:
			static const uint_fast8_t block_size = 16;

			Dec(const uint8_t* k)
			{
				Init(k, key);
				std::reverse(key, key + 32);
			}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};
	};
}

#endif
