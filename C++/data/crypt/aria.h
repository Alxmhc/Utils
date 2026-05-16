#ifndef H_ARIA
#define H_ARIA

#include <vector>

#include "../../math/base/math_.h"
#include "../u128.h"

namespace crypt
{
	//key sz = 16, 24, 32
	class ARIA
	{
		static void Init(const uint8_t* k, uint_fast8_t ksz, std::vector<UINT_<16>::uint> &key)
		{
			UINT_<16>::uint W[4];
			{
				const uint_fast8_t nr = (ksz >> 2) + 8;
				key.resize(nr + 1);

				const UINT_<16>::uint CK[3] = {
					UINT_<16>::uint(0xfe13abe8fa9a6ee0, 0x517cc1b727220a94),
					UINT_<16>::uint(0xff28b1d5ef5de2b0, 0x6db14acc9e21c820),
					UINT_<16>::uint(0x0324977504e8c90e, 0xdb92371d2126e970)
				};
				const uint_fast8_t n = (nr-12) >> 1;

				uint8_t r[16] = {};
				std::copy_n(k + 16, ksz - 16, r);
				W[0].pack<1, endianness::BIG_ENDIAN>(k);
				W[1].pack<1, endianness::BIG_ENDIAN>(r);
				W[1] ^= FO(W[0] ^ CK[(n+0)%3]);
				W[2] = W[1] ^ CK[(n+1)%3];
				SL(W[2]);
				A(W[2]);
				W[2] ^= W[0];
				W[3] = FO(W[2] ^ CK[(n+2)%3]);
				W[3] ^= W[1];
			}
			key[0]  = W[0] ^ rotr(W[1], 19);
			key[1]  = W[1] ^ rotr(W[2], 19);
			key[2]  = W[2] ^ rotr(W[3], 19);
			key[3]  = W[3] ^ rotr(W[0], 19);
			key[4]  = W[0] ^ rotr(W[1], 31);
			key[5]  = W[1] ^ rotr(W[2], 31);
			key[6]  = W[2] ^ rotr(W[3], 31);
			key[7]  = W[3] ^ rotr(W[0], 31);
			key[8]  = W[0] ^ rotl(W[1], 61);
			key[9]  = W[1] ^ rotl(W[2], 61);
			key[10] = W[2] ^ rotl(W[3], 61);
			key[11] = W[3] ^ rotl(W[0], 61);
			key[12] = W[0] ^ rotl(W[1], 31);
			if(ksz > 16)
			{
				key[13] = W[1] ^ rotl(W[2], 31);
				key[14] = W[2] ^ rotl(W[3], 31);
				if(ksz > 24)
				{
					key[15] = W[3] ^ rotl(W[0], 31);
					key[16] = W[0] ^ rotl(W[1], 19);
				}
			}
		}

		static const uint8_t SB1[256];
		static const uint8_t SB2[256];
		static const uint8_t SB3[256];
		static const uint8_t SB4[256];

		static void Av(const uint8_t* vi, UINT_<16>::uint &c)
		{
			uint8_t vo[16];
			vo[0]  = vi[3] ^ vi[4] ^ vi[6] ^ vi[8]  ^ vi[9]  ^ vi[13] ^ vi[14];
			vo[1]  = vi[2] ^ vi[5] ^ vi[7] ^ vi[8]  ^ vi[9]  ^ vi[12] ^ vi[15];
			vo[2]  = vi[1] ^ vi[4] ^ vi[6] ^ vi[10] ^ vi[11] ^ vi[12] ^ vi[15];
			vo[3]  = vi[0] ^ vi[5] ^ vi[7] ^ vi[10] ^ vi[11] ^ vi[13] ^ vi[14];
			vo[4]  = vi[0] ^ vi[2] ^ vi[5] ^ vi[8]  ^ vi[11] ^ vi[14] ^ vi[15];
			vo[5]  = vi[1] ^ vi[3] ^ vi[4] ^ vi[9]  ^ vi[10] ^ vi[14] ^ vi[15];
			vo[6]  = vi[0] ^ vi[2] ^ vi[7] ^ vi[9]  ^ vi[10] ^ vi[12] ^ vi[13];
			vo[7]  = vi[1] ^ vi[3] ^ vi[6] ^ vi[8]  ^ vi[11] ^ vi[12] ^ vi[13];
			vo[8]  = vi[0] ^ vi[1] ^ vi[4] ^ vi[7]  ^ vi[10] ^ vi[13] ^ vi[15];
			vo[9]  = vi[0] ^ vi[1] ^ vi[5] ^ vi[6]  ^ vi[11] ^ vi[12] ^ vi[14];
			vo[10] = vi[2] ^ vi[3] ^ vi[5] ^ vi[6]  ^ vi[8]  ^ vi[13] ^ vi[15];
			vo[11] = vi[2] ^ vi[3] ^ vi[4] ^ vi[7]  ^ vi[9]  ^ vi[12] ^ vi[14];
			vo[12] = vi[1] ^ vi[2] ^ vi[6] ^ vi[7]  ^ vi[9]  ^ vi[11] ^ vi[12];
			vo[13] = vi[0] ^ vi[3] ^ vi[6] ^ vi[7]  ^ vi[8]  ^ vi[10] ^ vi[13];
			vo[14] = vi[0] ^ vi[3] ^ vi[4] ^ vi[5]  ^ vi[9]  ^ vi[11] ^ vi[14];
			vo[15] = vi[1] ^ vi[2] ^ vi[4] ^ vi[5]  ^ vi[8]  ^ vi[10] ^ vi[15];
			c.pack<1, endianness::BIG_ENDIAN>(vo);
		}
		static void A(UINT_<16>::uint &c)
		{
			uint8_t v[16];
			c.unpack<1, endianness::BIG_ENDIAN>(v);
			Av(v, c);
		}
		static void SL(UINT_<16>::uint &c)
		{
			uint8_t v[16];
			c.unpack<1, endianness::BIG_ENDIAN>(v);
			v[0] = SB3[v[0]];
			v[1] = SB4[v[1]];
			v[2] = SB1[v[2]];
			v[3] = SB2[v[3]];
			v[4] = SB3[v[4]];
			v[5] = SB4[v[5]];
			v[6] = SB1[v[6]];
			v[7] = SB2[v[7]];
			v[8] = SB3[v[8]];
			v[9] = SB4[v[9]];
			v[10] = SB1[v[10]];
			v[11] = SB2[v[11]];
			v[12] = SB3[v[12]];
			v[13] = SB4[v[13]];
			v[14] = SB1[v[14]];
			v[15] = SB2[v[15]];
			c.pack<1, endianness::BIG_ENDIAN>(v);
		}
		static UINT_<16>::uint FO(const UINT_<16>::uint &c)
		{
			uint8_t v[16];
			c.unpack<1, endianness::BIG_ENDIAN>(v);
			v[0] = SB1[v[0]];
			v[1] = SB2[v[1]];
			v[2] = SB3[v[2]];
			v[3] = SB4[v[3]];
			v[4] = SB1[v[4]];
			v[5] = SB2[v[5]];
			v[6] = SB3[v[6]];
			v[7] = SB4[v[7]];
			v[8] = SB1[v[8]];
			v[9] = SB2[v[9]];
			v[10] = SB3[v[10]];
			v[11] = SB4[v[11]];
			v[12] = SB1[v[12]];
			v[13] = SB2[v[13]];
			v[14] = SB3[v[14]];
			v[15] = SB4[v[15]];
			UINT_<16>::uint r;
			Av(v, r);
			return r;
		}

		static void Process(const std::vector<UINT_<16>::uint> &key, uint8_t* r)
		{
			UINT_<16>::uint P;
			P.pack<1, endianness::BIG_ENDIAN>(r);
			const uint_fast8_t n = static_cast<uint_fast8_t>(key.size() - 1);
			uint_fast8_t i = 0;
			for(;;)
			{
				P ^= key[i++];
				P = FO(P);
				P ^= key[i++];
				SL(P);
				if(i == n)
					break;
				A(P);
			}
			P ^= key[i];
			P.unpack<1, endianness::BIG_ENDIAN>(r);
		}

	public:
		static const uint_fast8_t block_size = 16;

		class Enc
		{
			std::vector<UINT_<16>::uint> key;
		public:
			static const uint_fast8_t block_size = 16;

			Enc(const uint8_t* k, uint_fast8_t ksz)
			{
				Init(k, ksz, key);
			}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};

		class Dec
		{
			std::vector<UINT_<16>::uint> key;
		public:
			static const uint_fast8_t block_size = 16;

			Dec(const uint8_t* k, uint_fast8_t ksz)
			{
				Init(k, ksz, key);

				std::reverse(key.begin(), key.end());
				for(uint_fast8_t i = 1; i < key.size() - 1; i++)
				{
					A(key[i]);
				}
			}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};
	};
}

#endif
