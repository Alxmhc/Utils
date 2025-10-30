#ifndef H_MARS
#define H_MARS

#include "../../arr.h"
#include "../../math/base/math_.h"
#include "../pack.h"

namespace crypt
{
	//key sz = 16, 24, 32, 40, 48
	class MARS
	{
		static void Init(const uint8_t* k, uint_fast8_t ksz, uint32_t* key)
		{
			uint32_t T[15] = {};
			conv::pack<4, endianness::LITTLE_ENDIAN>(k, ksz, T);
			T[ksz/4] = ksz/4;
			for(uint_fast8_t j = 0; j < 4; j++)
			{
				for(uint_fast8_t i = 0; i < 15; i++)
				{
					T[i] ^= rotl(T[(i+8)%15] ^ T[(i+13)%15], 3) ^ (4*i + j);
				}
				for(uint_fast8_t k = 0; k < 4; k++)
				{
					for(uint_fast8_t i = 0; i < 15; i++)
					{
						T[i] = rotl(T[i] + S(T[(i+14)%15]), 9);
					}
				}
				for(uint_fast8_t i = 0; i < 10; i++)
				{
					key[10*j + i] = T[4*i%15];
				}
			}
			for(uint_fast8_t i = 5; i < 36; i += 2)
			{
				const uint32_t w = key[i] | 3;
				uint32_t M = (~w ^ (w<<1)) & (~w ^ (w>>1)) & 0x7ffffffe;

				M &= M>>1; M &= M>>2; M &= M>>4;
				M |= M<<1; M |= M<<2; M |= M<<4;
				M &= 0x7ffffffc;

				const uint32_t p = rotl(Sbox[265 + (key[i] & 3)], key[i-1] & 0x1f);
				key[i] = w ^ (p & M);
			}
		}

		static const uint32_t Sbox[512];

		static uint32_t S0(uint32_t c)
		{
			return Sbox[c & 0xff];
		}
		static uint32_t S1(uint32_t c)
		{
			return Sbox[(c & 0xff) | 0x100];
		}
		static uint32_t S(uint32_t c)
		{
			return Sbox[(c & 0x1ff)];
		}

	public:
		static const uint_fast8_t block_size = 16;

		class Enc
		{
			uint32_t key[40];
		public:
			static const uint_fast8_t block_size = 16;

			Enc(const uint8_t* k, uint_fast8_t ksz)
			{
				Init(k, ksz, key);
			}

			void process(uint8_t* r) const
			{
				uint32_t d[4];
				conv::pack<4, endianness::LITTLE_ENDIAN>(r, 16, d);
				v_add(d, key, 4);
				for(uint_fast8_t i = 0; i < 8; i++)
				{
					const uint32_t t = d[0];
					d[0] = (d[1] ^ S0(t)) + S1(t >> 8);
					d[1] = d[2] + S0(t >> 16);
					d[2] = d[3] ^ S1(t >> 24);
					d[3] = rotr(t, 24);

					const uint_fast8_t o = i & 3;
					if(o == 0)
					{
						d[3] += d[2];
					}
					else if(o == 1)
					{
						d[3] += d[0];
					}
				}
				for(uint_fast8_t i = 0; i < 16; i++)
				{
					uint32_t R = rotl(rotl(d[0], 13) * key[2*i+5], 5);
					uint32_t M = d[0] + key[2*i+4];
					uint32_t L = S(M) ^ R;
					M = rotl(M, R&0x1f);
					R = rotl(R, 5);
					L = rotl(L ^ R, R&0x1f);

					const uint32_t t = d[0];
					d[0] = (i<8) ? d[1] + L : d[1] ^ R;
					d[1] = d[2] + M;
					d[2] = (i<8) ? d[3] ^ R : d[3] + L;
					d[3] = rotl(t, 13);;
				}
				for(uint_fast8_t i = 0; i < 8; i++)
				{
					const uint_fast8_t o = i & 3;
					if(o == 2)
					{
						d[0] -= d[3];
					}
					else if(o == 3)
					{
						d[0] -= d[1];
					}
					const uint32_t t = d[0];
					d[0] = d[1] ^ S1(t);
					d[1] = d[2] - S0(t>>24);
					d[2] = (d[3] - S1(t>>16)) ^ S0(t>>8);
					d[3] = rotl(t, 24);
				}
				v_sub(d, key + 36, 4);
				conv::unpack<4, endianness::LITTLE_ENDIAN>(d, 4, r);
			}
		};

		class Dec
		{
			uint32_t key[40];
		public:
			static const uint_fast8_t block_size = 16;

			Dec(const uint8_t* k, uint_fast8_t ksz)
			{
				Init(k, ksz, key);
			}

			void process(uint8_t* r) const
			{
				uint32_t d[4];
				conv::pack<4, endianness::LITTLE_ENDIAN>(r, 16, d);
				v_add(d, key + 36, 4);
				for(uint_fast8_t i = 0; i < 8; i++)
				{
					const uint32_t t = d[3];
					d[3] = (d[2] ^ S0(t)) + S1(t>>8);
					d[2] = d[1] + S0(t>>16);
					d[1] = d[0] ^ S1(t>>24);
					d[0] = rotr(t, 24);

					const uint_fast8_t o = i & 3;
					if(o == 1)
					{
						d[0] += d[3];
					}
					else if(o == 0)
					{
						d[0] += d[1];
					}
				}
				for(uint_fast8_t i = 0; i < 16; i++)
				{
					uint32_t R = rotl(d[3] * key[2*(15-i)+5], 5);
					uint32_t M = rotr(d[3], 13) + key[2*(15-i)+4];
					uint32_t L = S(M) ^ R;
					M = rotl(M, R&0x1f);
					R = rotl(R, 5);
					L = rotl(L ^ R, R&0x1f);

					const uint32_t t = d[3];
					d[3] = (i<8) ? d[2] - L : d[2] ^ R;
					d[2] = d[1] - M;
					d[1] = (i<8) ? d[0] ^ R : d[0] - L;
					d[0] = rotr(t, 13);
				}
				for(uint_fast8_t i = 0; i < 8; i++)
				{
					const uint_fast8_t o = i & 3;
					if(o == 3)
					{
						d[3] -= d[2];
					}
					else if(o == 2)
					{
						d[3] -= d[0];
					}
					const uint32_t t = d[3];
					d[3] = d[2] ^ S1(t);
					d[2] = d[1] - S0(t >> 24);
					d[1] = (d[0] - S1(t >> 16)) ^ S0(t >> 8);
					d[0] = rotl(t, 24);
				}
				v_sub(d, key, 4);
				conv::unpack<4, endianness::LITTLE_ENDIAN>(d, 4, r);
			}
		};
	};
}

#endif
