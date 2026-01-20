#ifndef H_AES
#define H_AES

#include <vector>

#include "../../arr.h"

namespace crypt
{
	//key sz = 16, 24, 32
	class AES
	{
		static const uint8_t Sbox[256];

		static void Init(const uint8_t* k, uint_fast8_t ksz, std::vector<uint8_t> &key)
		{
			key.resize((ksz + 28) << 2);
			std::copy_n(k, ksz, key.begin());
			static const uint8_t Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
			uint8_t* t = key.data() + ksz - 4;
			for(uint_fast8_t i = 0; i < key.size() - ksz; i += 4)
			{
				const uint_fast8_t o = i % ksz;
				if(o == 0)
				{
					t[4] = Sbox[t[1]] ^ Rcon[i/ksz];
					t[5] = Sbox[t[2]];
					t[6] = Sbox[t[3]];
					t[7] = Sbox[t[0]];
				}
				else if((ksz > 24) && (o == 16))
				{
					t[4] = Sbox[t[0]];
					t[5] = Sbox[t[1]];
					t[6] = Sbox[t[2]];
					t[7] = Sbox[t[3]];
				}
				else
				{
					std::copy(t, t + 4, t + 4);
				}
				t += 4;
				t[0] ^= key[i];
				t[1] ^= key[i+1];
				t[2] ^= key[i+2];
				t[3] ^= key[i+3];
			}
		}
	public:
		static const uint_fast8_t block_size = 16;

		class Enc
		{
			std::vector<uint8_t> key;

			static const uint8_t Mul3[256];

			static void SubShift(uint8_t* r)
			{
				const uint8_t t[16] = {
				Sbox[r[0]],  Sbox[r[5]],  Sbox[r[10]], Sbox[r[15]],
				Sbox[r[4]],  Sbox[r[9]],  Sbox[r[14]], Sbox[r[3]],
				Sbox[r[8]],  Sbox[r[13]], Sbox[r[2]],  Sbox[r[7]],
				Sbox[r[12]], Sbox[r[1]],  Sbox[r[6]],  Sbox[r[11]]};
				std::copy_n(t, 16, r);
			}
			static void Mix(uint8_t* r)
			{
				for(uint_fast8_t i = 0; i < 16; i += 4)
				{
					const uint8_t d0 = r[i] ^ r[i + 1];
					const uint8_t d1 = r[i + 1] ^ r[i + 2];
					const uint8_t d2 = r[i + 2] ^ r[i + 3];
					const uint8_t d3 = r[i + 3] ^ r[i];
					r[i]   ^= d2 ^ Mul3[d0];
					r[i+1] ^= d3 ^ Mul3[d1];
					r[i+2] ^= d0 ^ Mul3[d2];
					r[i+3] ^= d1 ^ Mul3[d3];
				}
			}
		public:
			static const uint_fast8_t block_size = 16;

			Enc(const uint8_t* k, uint_fast8_t ksz)
			{
				Init(k, ksz, key);
			}

			void process(uint8_t* r) const
			{
				v_xor(r, key.data(), 16);
				std::size_t i = 16;
				for(; i < key.size() - 16; i += 16)
				{
					SubShift(r);
					Mix(r);
					v_xor(r, key.data() + i, 16);
				}
				SubShift(r);
				v_xor(r, key.data() + i, 16);
			}
		};

		class Dec
		{
			std::vector<uint8_t> key;

			static const uint8_t SboxI[256];
			static const uint8_t Mul9[256];
			static const uint8_t Mulb[256];
			static const uint8_t Muld[256];
			static const uint8_t Mule[256];

			static void SubShiftI(uint8_t* r)
			{
				const uint8_t t[16] = {
				SboxI[r[0]],  SboxI[r[13]], SboxI[r[10]], SboxI[r[7]],
				SboxI[r[4]],  SboxI[r[1]],  SboxI[r[14]], SboxI[r[11]],
				SboxI[r[8]],  SboxI[r[5]],  SboxI[r[2]],  SboxI[r[15]],
				SboxI[r[12]], SboxI[r[9]],  SboxI[r[6]],  SboxI[r[3]]};
				std::copy_n(t, 16, r);
			}
			static void MixI(uint8_t* r)
			{
				for(uint_fast8_t i = 0; i < 16; i += 4)
				{
					const uint8_t d[4] = {r[i], r[i+1], r[i+2], r[i+3]};
					r[i]   = Mule[d[0]] ^ Mulb[d[1]] ^ Muld[d[2]] ^ Mul9[d[3]];
					r[i+1] = Mul9[d[0]] ^ Mule[d[1]] ^ Mulb[d[2]] ^ Muld[d[3]];
					r[i+2] = Muld[d[0]] ^ Mul9[d[1]] ^ Mule[d[2]] ^ Mulb[d[3]];
					r[i+3] = Mulb[d[0]] ^ Muld[d[1]] ^ Mul9[d[2]] ^ Mule[d[3]];
				}
			}
		public:
			static const uint_fast8_t block_size = 16;

			Dec(const uint8_t* k, uint_fast8_t ksz)
			{
				Init(k, ksz, key);
			}

			void process(uint8_t* r) const
			{
				std::size_t i = key.size() - 16;
				v_xor(r, key.data() + i, 16);
				i -= 16;
				SubShiftI(r);
				for(; i > 0; i -= 16)
				{
					v_xor(r, key.data() + i, 16);
					MixI(r);
					SubShiftI(r);
				}
				v_xor(r, key.data(), 16);
			}
		};
	};
}

#endif
