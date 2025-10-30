#ifndef H_CAMELLIA
#define H_CAMELLIA

#include <vector>

#include "../../math/base/math_.h"
#include "../pack.h"

namespace crypt
{
	//key sz = 16, 24, 32
	class Camellia
	{
		static void Init(const uint8_t* k, uint_fast8_t ksz, UINT_<16>::uint* key)
		{
			const auto KL = bconv<1, 16, endianness::BIG_ENDIAN>::pack(k);
			UINT_<16>::uint KR;
			if(ksz == 24)
			{
				const uint64_t t = bconv<1, 8, endianness::BIG_ENDIAN>::pack(k + 16);
				KR = UINT_<16>::uint(~t, t);
			}
			else if(ksz == 32)
			{
				KR = bconv<1, 16, endianness::BIG_ENDIAN>::pack(k + 16);
			}

			UINT_<16>::uint KA = KL ^ KR;
			F1(KA, UINT_<16>::uint(0xb67ae8584caa73b2, 0xa09e667f3bcc908b));
			KA ^= KL;
			F1(KA, UINT_<16>::uint(0x54ff53a5f1d36f1c, 0xc6ef372fe94f82be));

			key[0] = KL;
			key[4] = rotl(KA, 15);
			if(ksz == 16)
			{
				key[3] = rotl(KL, 15);
				key[5] = rotl(KL, 45);
				key[6] = UINT_<16>::uint(rotl(KL, 60).getL(), rotl(KA, 45).getH());
				key[7] = rotl(KA, 60);
				key[8] = rotl(KL, 94);
				key[9] = rotl(KA, 94);
				key[10] = rotl(KL, 111);
				key[14] = rotl(KA, 30);
				key[15] = rotl(KL, 77);
			}
			else
			{
				key[3] = rotl(KR, 15);
				key[6] = rotl(KL, 45);
				key[7] = rotl(KA, 45);
				key[8] = rotl(KR, 60);
				key[10] = rotl(KL, 77);
				key[11] = rotl(KR, 94);
				key[12] = rotl(KA, 94);
				key[13] = rotl(KL, 111);
				key[14] = rotl(KR, 30);
				key[15] = rotl(KL, 60);
				key[16] = rotl(KA, 77);

				KA ^= KR;
				F1(KA, UINT_<16>::uint(0xb05688c2b3e6c1fd, 0x10e527fade682d1d));

				key[5] = rotl(KA, 30);
				key[9] = rotl(KA, 60);
			}
			key[1] = rotl(KA, 111);
			key[2] = KA;
		}

		static const uint8_t SB[256];

		static void F(const uint8_t* x1, uint8_t* x2)
		{
			uint8_t t[8];
			t[0] = SB[x1[0]];
			t[1] = rotl(SB[x1[1]], 1);
			t[2] = rotl(SB[x1[2]], 7);
			t[3] = SB[rotl(x1[3], 1)];
			t[4] = rotl(SB[x1[4]], 1);
			t[5] = rotl(SB[x1[5]], 7);
			t[6] = SB[rotl(x1[6], 1)];
			t[7] = SB[x1[7]];
			x2[0] ^= t[0] ^ t[2] ^ t[3] ^ t[5] ^ t[6] ^ t[7];
			x2[1] ^= t[0] ^ t[1] ^ t[3] ^ t[4] ^ t[6] ^ t[7];
			x2[2] ^= t[0] ^ t[1] ^ t[2] ^ t[4] ^ t[5] ^ t[7];
			x2[3] ^= t[1] ^ t[2] ^ t[3] ^ t[4] ^ t[5] ^ t[6];
			x2[4] ^= t[0] ^ t[1] ^ t[5] ^ t[6] ^ t[7];
			x2[5] ^= t[1] ^ t[2] ^ t[4] ^ t[6] ^ t[7];
			x2[6] ^= t[2] ^ t[3] ^ t[4] ^ t[5] ^ t[7];
			x2[7] ^= t[0] ^ t[3] ^ t[4] ^ t[5] ^ t[6];
		}

		static void F1(UINT_<16>::uint &D, UINT_<16>::uint K)
		{
			D ^= K;
			uint8_t x[16];
			bconv<1, 16, endianness::BIG_ENDIAN>::unpack(D, x);
			F(x, x + 8);
			F(x + 8, x);
			D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(x);
			D ^= K;
		}
		static void F2(UINT_<16>::uint &D, UINT_<16>::uint K)
		{
			D ^= K;
			uint8_t x[16];
			bconv<1, 16, endianness::BIG_ENDIAN>::unpack(D, x);
			F(x + 8, x);
			F(x, x + 8);
			D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(x);
			D ^= K;
		}

		static void FL1(UINT_<16>::uint &D, UINT_<16>::uint K)
		{
			uint32_t x[4], k[4];
			bconv<4, 4, endianness::BIG_ENDIAN>::unpack(D, x);
			bconv<4, 4, endianness::BIG_ENDIAN>::unpack(K, k);
			x[1] ^= rotl(x[0] & k[0], 1);
			x[0] ^= (x[1] | k[1]);
			x[2] ^= (x[3] | k[3]);
			x[3] ^= rotl(x[2] & k[2], 1);
			D = bconv<4, 4, endianness::BIG_ENDIAN>::pack(x);
		}
		static void FL2(UINT_<16>::uint &D, UINT_<16>::uint K)
		{
			uint32_t x[4], k[4];
			bconv<4, 4, endianness::BIG_ENDIAN>::unpack(D, x);
			bconv<4, 4, endianness::BIG_ENDIAN>::unpack(K, k);
			x[3] ^= rotl(x[2] & k[2], 1);
			x[2] ^= (x[3] | k[3]);
			x[0] ^= (x[1] | k[1]);
			x[1] ^= rotl(x[0] & k[0], 1);
			D = bconv<4, 4, endianness::BIG_ENDIAN>::pack(x);
		}

	public:
		static const uint_fast8_t block_size = 16;

		class Enc
		{
			UINT_<16>::uint key[17];
			bool e;
		public:
			static const uint_fast8_t block_size = 16;

			Enc(const uint8_t* k, uint_fast8_t ksz)
			{
				e = (ksz != 16);
				Init(k, ksz, key);
			}

			void process(uint8_t* r) const
			{
				auto D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(r);
				D ^= key[0];
				F1(D, key[2]);
				F1(D, key[3]);
				F1(D, key[4]);
				FL1(D, key[14]);
				F1(D, key[5]);
				F1(D, key[6]);
				F1(D, key[7]);
				FL1(D, key[15]);
				F1(D, key[8]);
				F1(D, key[9]);
				F1(D, key[10]);
				if(e)
				{
					FL1(D, key[16]);
					F1(D, key[11]);
					F1(D, key[12]);
					F1(D, key[13]);
				}
				D = UINT_<16>::uint(D.getH(), D.getL());
				D ^= key[1];
				bconv<1, 16, endianness::BIG_ENDIAN>::unpack(D, r);
			}
		};

		class Dec
		{
			UINT_<16>::uint key[17];
			bool e;
		public:
			static const uint_fast8_t block_size = 16;

			Dec(const uint8_t* k, uint_fast8_t ksz)
			{
				e = (ksz != 16);
				Init(k, ksz, key);
			}

			void process(uint8_t* r) const
			{
				auto D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(r);
				D ^= key[1];
				D = UINT_<16>::uint(D.getH(), D.getL());
				if(e)
				{
					F2(D, key[13]);
					F2(D, key[12]);
					F2(D, key[11]);
					FL2(D, key[16]);
				}
				F2(D, key[10]);
				F2(D, key[9]);
				F2(D, key[8]);
				FL2(D, key[15]);
				F2(D, key[7]);
				F2(D, key[6]);
				F2(D, key[5]);
				FL2(D, key[14]);
				F2(D, key[4]);
				F2(D, key[3]);
				F2(D, key[2]);
				D ^= key[0];
				bconv<1, 16, endianness::BIG_ENDIAN>::unpack(D, r);
			}
		};
	};
}

#endif
