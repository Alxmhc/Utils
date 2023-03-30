#ifndef H_CAMELLIA
#define H_CAMELLIA

#include <vector>
#include "../../math/base/math_.h"
#include "../pack.h"

namespace crypt
{
	class Camellia
	{
		UINT_<16>::uint_ key[17];

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

		static void F1(UINT_<16>::uint_ &D, UINT_<16>::uint_ K)
		{
			D ^= K;
			uint8_t x[16];
			bconv<1, 16, endianness::BIG_ENDIAN>::unpack(D, x);
			F(x, x + 8);
			F(x + 8, x);
			D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(x);
			D ^= K;
		}
		static void F2(UINT_<16>::uint_ &D, UINT_<16>::uint_ K)
		{
			D ^= K;
			uint8_t x[16];
			bconv<1, 16, endianness::BIG_ENDIAN>::unpack(D, x);
			F(x + 8, x);
			F(x, x + 8);
			D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(x);
			D ^= K;
		}

		static void FL1(UINT_<16>::uint_ &D, UINT_<16>::uint_ K)
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
		static void FL2(UINT_<16>::uint_ &D, UINT_<16>::uint_ K)
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

		class en
		{
			const UINT_<16>::uint_* key;
			const bool e;
		public:
			static const uint_fast8_t block_size = 16;
			en(const UINT_<16>::uint_* k, bool ext) : key(k), e(ext) {}

			void process(uint8_t* r) const
			{
				UINT_<16>::uint_ D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(r);
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
				D = UINT_<16>::uint_(D.getH(), D.getL());
				D ^= key[1];
				bconv<1, 16, endianness::BIG_ENDIAN>::unpack(D, r);
			}
		};

		class de
		{
			const UINT_<16>::uint_* key;
			const bool e;
		public:
			static const uint_fast8_t block_size = 16;
			de(const UINT_<16>::uint_* k, bool ext) : key(k), e(ext) {}

			void process(uint8_t* r) const
			{
				UINT_<16>::uint_ D = bconv<1, 16, endianness::BIG_ENDIAN>::pack(r);
				D ^= key[1];
				D = UINT_<16>::uint_(D.getH(), D.getL());
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
	public:
		static const uint_fast8_t block_size = 16;

		en Enc;
		de Dec;

		Camellia(const uint8_t* k, uint_fast8_t ksz) : Enc(key, ksz != 16), Dec(key, ksz != 16)
		{
			UINT_<16>::uint_ KL = bconv<1, 16, endianness::BIG_ENDIAN>::pack(k);
			UINT_<16>::uint_ KR;
			if(ksz == 24)
			{
				const uint64_t t = bconv<1, 8, endianness::BIG_ENDIAN>::pack(k + 16);
				KR = UINT_<16>::uint_(~t, t);
			}
			else if(ksz == 32)
			{
				KR = bconv<1, 16, endianness::BIG_ENDIAN>::pack(k + 16);
			}

			UINT_<16>::uint_ KA = KL ^ KR;
			F1(KA, UINT_<16>::uint_(0xb67ae8584caa73b2, 0xa09e667f3bcc908b));
			KA ^= KL;
			F1(KA, UINT_<16>::uint_(0x54ff53a5f1d36f1c, 0xc6ef372fe94f82be));

			key[0] = KL;
			key[4] = rotl(KA, 15);
			if(ksz == 16)
			{
				key[3] = rotl(KL, 15);
				key[5] = rotl(KL, 45);
				key[6] = UINT_<16>::uint_(rotl(KL, 60).getL(), rotl(KA, 45).getH());
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
				F1(KA, UINT_<16>::uint_(0xb05688c2b3e6c1fd, 0x10e527fade682d1d));

				key[5] = rotl(KA, 30);
				key[9] = rotl(KA, 60);
			}
			key[1] = rotl(KA, 111);
			key[2] = KA;
		}
	};

	const uint8_t Camellia::SB[256] = {
		0x70, 0x82, 0x2c, 0xec, 0xb3, 0x27, 0xc0, 0xe5, 0xe4, 0x85, 0x57, 0x35, 0xea, 0x0c, 0xae, 0x41,
		0x23, 0xef, 0x6b, 0x93, 0x45, 0x19, 0xa5, 0x21, 0xed, 0x0e, 0x4f, 0x4e, 0x1d, 0x65, 0x92, 0xbd,
		0x86, 0xb8, 0xaf, 0x8f, 0x7c, 0xeb, 0x1f, 0xce, 0x3e, 0x30, 0xdc, 0x5f, 0x5e, 0xc5, 0x0b, 0x1a,
		0xa6, 0xe1, 0x39, 0xca, 0xd5, 0x47, 0x5d, 0x3d, 0xd9, 0x01, 0x5a, 0xd6, 0x51, 0x56, 0x6c, 0x4d,
		0x8b, 0x0d, 0x9a, 0x66, 0xfb, 0xcc, 0xb0, 0x2d, 0x74, 0x12, 0x2b, 0x20, 0xf0, 0xb1, 0x84, 0x99,
		0xdf, 0x4c, 0xcb, 0xc2, 0x34, 0x7e, 0x76, 0x05, 0x6d, 0xb7, 0xa9, 0x31, 0xd1, 0x17, 0x04, 0xd7,
		0x14, 0x58, 0x3a, 0x61, 0xde, 0x1b, 0x11, 0x1c, 0x32, 0x0f, 0x9c, 0x16, 0x53, 0x18, 0xf2, 0x22,
		0xfe, 0x44, 0xcf, 0xb2, 0xc3, 0xb5, 0x7a, 0x91, 0x24, 0x08, 0xe8, 0xa8, 0x60, 0xfc, 0x69, 0x50,
		0xaa, 0xd0, 0xa0, 0x7d, 0xa1, 0x89, 0x62, 0x97, 0x54, 0x5b, 0x1e, 0x95, 0xe0, 0xff, 0x64, 0xd2,
		0x10, 0xc4, 0x00, 0x48, 0xa3, 0xf7, 0x75, 0xdb, 0x8a, 0x03, 0xe6, 0xda, 0x09, 0x3f, 0xdd, 0x94,
		0x87, 0x5c, 0x83, 0x02, 0xcd, 0x4a, 0x90, 0x33, 0x73, 0x67, 0xf6, 0xf3, 0x9d, 0x7f, 0xbf, 0xe2,
		0x52, 0x9b, 0xd8, 0x26, 0xc8, 0x37, 0xc6, 0x3b, 0x81, 0x96, 0x6f, 0x4b, 0x13, 0xbe, 0x63, 0x2e,
		0xe9, 0x79, 0xa7, 0x8c, 0x9f, 0x6e, 0xbc, 0x8e, 0x29, 0xf5, 0xf9, 0xb6, 0x2f, 0xfd, 0xb4, 0x59,
		0x78, 0x98, 0x06, 0x6a, 0xe7, 0x46, 0x71, 0xba, 0xd4, 0x25, 0xab, 0x42, 0x88, 0xa2, 0x8d, 0xfa,
		0x72, 0x07, 0xb9, 0x55, 0xf8, 0xee, 0xac, 0x0a, 0x36, 0x49, 0x2a, 0x68, 0x3c, 0x38, 0xf1, 0xa4,
		0x40, 0x28, 0xd3, 0x7b, 0xbb, 0xc9, 0x43, 0xc1, 0x15, 0xe3, 0xad, 0xf4, 0x77, 0xc7, 0x80, 0x9e
	};
}

#endif
