#ifndef H_ARIA
#define H_ARIA

#include <vector>
#include "../../math/base/math_.h"
#include "../pack.h"

namespace crypt
{
	class ARIA
	{
		std::vector<UINT_<16>::uint_> keyEnc;
		std::vector<UINT_<16>::uint_> keyDec;

		static const uint8_t SB1[256];
		static const uint8_t SB2[256];
		static const uint8_t SB3[256];
		static const uint8_t SB4[256];

		static void Av(const uint8_t* vi, uint8_t* vo)
		{
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
		}
		static UINT_<16>::uint_ A(const UINT_<16>::uint_ &c)
		{
			uint8_t vi[16], vo[16];
			bconv<16, endianness::BIG_ENDIAN>::unpack(c, vi);
			Av(vi, vo);
			return bconv<16, endianness::BIG_ENDIAN>::pack(vo);
		}
		static UINT_<16>::uint_ SL(const UINT_<16>::uint_ &c)
		{
			uint8_t vi[16];
			bconv<16, endianness::BIG_ENDIAN>::unpack(c, vi);
			uint8_t vo[16] = {
			SB3[vi[0]],  SB4[vi[1]],  SB1[vi[2]],  SB2[vi[3]],
			SB3[vi[4]],  SB4[vi[5]],  SB1[vi[6]],  SB2[vi[7]],
			SB3[vi[8]],  SB4[vi[9]],  SB1[vi[10]], SB2[vi[11]],
			SB3[vi[12]], SB4[vi[13]], SB1[vi[14]], SB2[vi[15]]};
			return bconv<16, endianness::BIG_ENDIAN>::pack(vo);
		}
		static UINT_<16>::uint_ FO(const UINT_<16>::uint_ &c)
		{
			uint8_t v[16];
			bconv<16, endianness::BIG_ENDIAN>::unpack(c, v);
			uint8_t vo[16] = {
			SB1[v[0]],  SB2[v[1]],  SB3[v[2]],  SB4[v[3]],
			SB1[v[4]],  SB2[v[5]],  SB3[v[6]],  SB4[v[7]],
			SB1[v[8]],  SB2[v[9]],  SB3[v[10]], SB4[v[11]],
			SB1[v[12]], SB2[v[13]], SB3[v[14]], SB4[v[15]]};
			Av(vo, v);
			return bconv<16, endianness::BIG_ENDIAN>::pack(v);
		}

		static void Process(const std::vector<UINT_<16>::uint_> &key, uint8_t* r)
		{
			auto P = bconv<16, endianness::BIG_ENDIAN>::pack(r);
			const uint_fast8_t n = static_cast<uint_fast8_t>(key.size() - 1);
			uint_fast8_t i = 0;
			for(;;)
			{
				P = FO(P ^ key[i++]);
				P = SL(P ^ key[i++]);
				if(i == n)
					break;
				P = A(P);
			}
			P ^= key[i];
			bconv<16, endianness::BIG_ENDIAN>::unpack(P, r);
		}

		class en
		{
			const std::vector<UINT_<16>::uint_>* key;
		public:
			static const uint_fast8_t block_size = 16;
			en(const std::vector<UINT_<16>::uint_> &k) : key(&k) {}

			void process(uint8_t* r) const
			{
				Process(*key, r);
			}
		};

		class de
		{
			const std::vector<UINT_<16>::uint_>* key;
		public:
			static const uint_fast8_t block_size = 16;
			de(const std::vector<UINT_<16>::uint_> &k) : key(&k) {}

			void process(uint8_t* r) const
			{
				Process(*key, r);
			}
		};
	public:
		static const uint_fast8_t block_size = 16;

		en Enc;
		de Dec;

		ARIA(const uint8_t* k, uint_fast8_t ksz) : Enc(keyEnc), Dec(keyDec)
		{
			uint8_t r[16] = {};
			memcpy(r, k + 16, ksz - 16);

			UINT_<16>::uint_ KL = bconv<16, endianness::BIG_ENDIAN>::pack(k);
			UINT_<16>::uint_ KR = bconv<16, endianness::BIG_ENDIAN>::pack(r);

			const UINT_<16>::uint_ CK[3] = {
				UINT_<16>::uint_(0xfe13abe8fa9a6ee0, 0x517cc1b727220a94),
				UINT_<16>::uint_(0xff28b1d5ef5de2b0, 0x6db14acc9e21c820),
				UINT_<16>::uint_(0x0324977504e8c90e, 0xdb92371d2126e970)
			};

			const uint_fast8_t nr = (ksz >> 2) + 8;

			const uint_fast8_t n = (nr-12) >> 1;
			UINT_<16>::uint_ W[4];
			W[0] = KL;
			W[1] = FO(W[0] ^ CK[(n+0)%3]) ^ KR;
			W[2] = A(SL(W[1] ^ CK[(n+1)%3])) ^ W[0];
			W[3] = FO(W[2] ^ CK[(n+2)%3]) ^ W[1];

			keyEnc.resize(nr + 1);
			keyEnc[0]  = W[0] ^ rotr(W[1], 19);
			keyEnc[1]  = W[1] ^ rotr(W[2], 19);
			keyEnc[2]  = W[2] ^ rotr(W[3], 19);
			keyEnc[3]  = W[3] ^ rotr(W[0], 19);
			keyEnc[4]  = W[0] ^ rotr(W[1], 31);
			keyEnc[5]  = W[1] ^ rotr(W[2], 31);
			keyEnc[6]  = W[2] ^ rotr(W[3], 31);
			keyEnc[7]  = W[3] ^ rotr(W[0], 31);
			keyEnc[8]  = W[0] ^ rotl(W[1], 61);
			keyEnc[9]  = W[1] ^ rotl(W[2], 61);
			keyEnc[10] = W[2] ^ rotl(W[3], 61);
			keyEnc[11] = W[3] ^ rotl(W[0], 61);
			keyEnc[12] = W[0] ^ rotl(W[1], 31);
			if(nr > 12)
			{
				keyEnc[13] = W[1] ^ rotl(W[2], 31);
				keyEnc[14] = W[2] ^ rotl(W[3], 31);
				if(nr > 14)
				{
					keyEnc[15] = W[3] ^ rotl(W[0], 31);
					keyEnc[16] = W[0] ^ rotl(W[1], 19);
				}
			}

			keyDec.resize(nr + 1);
			keyDec[0] = keyEnc[nr];
			for(uint_fast8_t i = 1; i < nr; i++)
			{
				keyDec[i] = A(keyEnc[nr-i]);
			}
			keyDec[nr] = keyEnc[0];
		}
	};

	const uint8_t ARIA::SB1[256] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	};

	const uint8_t ARIA::SB2[256] = {
		0xe2, 0x4e, 0x54, 0xfc, 0x94, 0xc2, 0x4a, 0xcc, 0x62, 0x0d, 0x6a, 0x46, 0x3c, 0x4d, 0x8b, 0xd1,
		0x5e, 0xfa, 0x64, 0xcb, 0xb4, 0x97, 0xbe, 0x2b, 0xbc, 0x77, 0x2e, 0x03, 0xd3, 0x19, 0x59, 0xc1,
		0x1d, 0x06, 0x41, 0x6b, 0x55, 0xf0, 0x99, 0x69, 0xea, 0x9c, 0x18, 0xae, 0x63, 0xdf, 0xe7, 0xbb,
		0x00, 0x73, 0x66, 0xfb, 0x96, 0x4c, 0x85, 0xe4, 0x3a, 0x09, 0x45, 0xaa, 0x0f, 0xee, 0x10, 0xeb,
		0x2d, 0x7f, 0xf4, 0x29, 0xac, 0xcf, 0xad, 0x91, 0x8d, 0x78, 0xc8, 0x95, 0xf9, 0x2f, 0xce, 0xcd,
		0x08, 0x7a, 0x88, 0x38, 0x5c, 0x83, 0x2a, 0x28, 0x47, 0xdb, 0xb8, 0xc7, 0x93, 0xa4, 0x12, 0x53,
		0xff, 0x87, 0x0e, 0x31, 0x36, 0x21, 0x58, 0x48, 0x01, 0x8e, 0x37, 0x74, 0x32, 0xca, 0xe9, 0xb1,
		0xb7, 0xab, 0x0c, 0xd7, 0xc4, 0x56, 0x42, 0x26, 0x07, 0x98, 0x60, 0xd9, 0xb6, 0xb9, 0x11, 0x40,
		0xec, 0x20, 0x8c, 0xbd, 0xa0, 0xc9, 0x84, 0x04, 0x49, 0x23, 0xf1, 0x4f, 0x50, 0x1f, 0x13, 0xdc,
		0xd8, 0xc0, 0x9e, 0x57, 0xe3, 0xc3, 0x7b, 0x65, 0x3b, 0x02, 0x8f, 0x3e, 0xe8, 0x25, 0x92, 0xe5,
		0x15, 0xdd, 0xfd, 0x17, 0xa9, 0xbf, 0xd4, 0x9a, 0x7e, 0xc5, 0x39, 0x67, 0xfe, 0x76, 0x9d, 0x43,
		0xa7, 0xe1, 0xd0, 0xf5, 0x68, 0xf2, 0x1b, 0x34, 0x70, 0x05, 0xa3, 0x8a, 0xd5, 0x79, 0x86, 0xa8,
		0x30, 0xc6, 0x51, 0x4b, 0x1e, 0xa6, 0x27, 0xf6, 0x35, 0xd2, 0x6e, 0x24, 0x16, 0x82, 0x5f, 0xda,
		0xe6, 0x75, 0xa2, 0xef, 0x2c, 0xb2, 0x1c, 0x9f, 0x5d, 0x6f, 0x80, 0x0a, 0x72, 0x44, 0x9b, 0x6c,
		0x90, 0x0b, 0x5b, 0x33, 0x7d, 0x5a, 0x52, 0xf3, 0x61, 0xa1, 0xf7, 0xb0, 0xd6, 0x3f, 0x7c, 0x6d,
		0xed, 0x14, 0xe0, 0xa5, 0x3d, 0x22, 0xb3, 0xf8, 0x89, 0xde, 0x71, 0x1a, 0xaf, 0xba, 0xb5, 0x81
	};

	const uint8_t ARIA::SB3[256] = {
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
	};

	const uint8_t ARIA::SB4[256] = {
		0x30, 0x68, 0x99, 0x1b, 0x87, 0xb9, 0x21, 0x78, 0x50, 0x39, 0xdb, 0xe1, 0x72, 0x09, 0x62, 0x3c,
		0x3e, 0x7e, 0x5e, 0x8e, 0xf1, 0xa0, 0xcc, 0xa3, 0x2a, 0x1d, 0xfb, 0xb6, 0xd6, 0x20, 0xc4, 0x8d,
		0x81, 0x65, 0xf5, 0x89, 0xcb, 0x9d, 0x77, 0xc6, 0x57, 0x43, 0x56, 0x17, 0xd4, 0x40, 0x1a, 0x4d,
		0xc0, 0x63, 0x6c, 0xe3, 0xb7, 0xc8, 0x64, 0x6a, 0x53, 0xaa, 0x38, 0x98, 0x0c, 0xf4, 0x9b, 0xed,
		0x7f, 0x22, 0x76, 0xaf, 0xdd, 0x3a, 0x0b, 0x58, 0x67, 0x88, 0x06, 0xc3, 0x35, 0x0d, 0x01, 0x8b,
		0x8c, 0xc2, 0xe6, 0x5f, 0x02, 0x24, 0x75, 0x93, 0x66, 0x1e, 0xe5, 0xe2, 0x54, 0xd8, 0x10, 0xce,
		0x7a, 0xe8, 0x08, 0x2c, 0x12, 0x97, 0x32, 0xab, 0xb4, 0x27, 0x0a, 0x23, 0xdf, 0xef, 0xca, 0xd9,
		0xb8, 0xfa, 0xdc, 0x31, 0x6b, 0xd1, 0xad, 0x19, 0x49, 0xbd, 0x51, 0x96, 0xee, 0xe4, 0xa8, 0x41,
		0xda, 0xff, 0xcd, 0x55, 0x86, 0x36, 0xbe, 0x61, 0x52, 0xf8, 0xbb, 0x0e, 0x82, 0x48, 0x69, 0x9a,
		0xe0, 0x47, 0x9e, 0x5c, 0x04, 0x4b, 0x34, 0x15, 0x79, 0x26, 0xa7, 0xde, 0x29, 0xae, 0x92, 0xd7,
		0x84, 0xe9, 0xd2, 0xba, 0x5d, 0xf3, 0xc5, 0xb0, 0xbf, 0xa4, 0x3b, 0x71, 0x44, 0x46, 0x2b, 0xfc,
		0xeb, 0x6f, 0xd5, 0xf6, 0x14, 0xfe, 0x7c, 0x70, 0x5a, 0x7d, 0xfd, 0x2f, 0x18, 0x83, 0x16, 0xa5,
		0x91, 0x1f, 0x05, 0x95, 0x74, 0xa9, 0xc1, 0x5b, 0x4a, 0x85, 0x6d, 0x13, 0x07, 0x4f, 0x4e, 0x45,
		0xb2, 0x0f, 0xc9, 0x1c, 0xa6, 0xbc, 0xec, 0x73, 0x90, 0x7b, 0xcf, 0x59, 0x8f, 0xa1, 0xf9, 0x2d,
		0xf2, 0xb1, 0x00, 0x94, 0x37, 0x9f, 0xd0, 0x2e, 0x9c, 0x6e, 0x28, 0x3f, 0x80, 0xf0, 0x3d, 0xd3,
		0x25, 0x8a, 0xb5, 0xe7, 0x42, 0xb3, 0xc7, 0xea, 0xf7, 0x4c, 0x11, 0x33, 0x03, 0xa2, 0xac, 0x60
	};
}

#endif
