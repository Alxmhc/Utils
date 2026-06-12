#ifndef H_AES
#define H_AES

#include <vector>
#include <algorithm>

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
				uint8_t t[16];
				std::copy_n(r, 16, t);

				r[0]  = Sbox[t[0]];
				r[1]  = Sbox[t[5]];
				r[2]  = Sbox[t[10]];
				r[3]  = Sbox[t[15]];
				r[4]  = Sbox[t[4]];
				r[5]  = Sbox[t[9]];
				r[6]  = Sbox[t[14]];
				r[7]  = Sbox[t[3]];
				r[8]  = Sbox[t[8]];
				r[9]  = Sbox[t[13]];
				r[10] = Sbox[t[2]];
				r[11] = Sbox[t[7]];
				r[12] = Sbox[t[12]];
				r[13] = Sbox[t[1]];
				r[14] = Sbox[t[6]];
				r[15] = Sbox[t[11]];
			}
			static void ShiftMix(uint8_t* r)
			{
				SubShift(r);
				for(uint_fast8_t i = 0; i < 16; i += 4)
				{
					const uint8_t t0 = r[i]   ^ Mul3[r[i+2]];
					const uint8_t t1 = r[i+1] ^ Mul3[r[i+3]];
					const uint8_t t2 = r[i+2] ^ Mul3[r[i]];
					const uint8_t t3 = r[i+3] ^ Mul3[r[i+1]];
					r[i]   ^= t2 ^ t3;
					r[i+1] ^= t3 ^ t0;
					r[i+2] ^= t0 ^ t1;
					r[i+3] ^= t1 ^ t2;
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
					ShiftMix(r);
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
				uint8_t t[16];
				std::copy_n(r, 16, t);

				r[0]  = SboxI[t[0]];
				r[1]  = SboxI[t[13]];
				r[2]  = SboxI[t[10]];
				r[3]  = SboxI[t[7]];
				r[4]  = SboxI[t[4]];
				r[5]  = SboxI[t[1]];
				r[6]  = SboxI[t[14]];
				r[7]  = SboxI[t[11]];
				r[8]  = SboxI[t[8]];
				r[9]  = SboxI[t[5]];
				r[10] = SboxI[t[2]];
				r[11] = SboxI[t[15]];
				r[12] = SboxI[t[12]];
				r[13] = SboxI[t[9]];
				r[14] = SboxI[t[6]];
				r[15] = SboxI[t[3]];
			}
			static void MixShiftI(uint8_t* r)
			{
				for(uint_fast8_t i = 0; i < 16; i += 4)
				{
					const uint8_t t0 = r[i];
					const uint8_t t1 = r[i+1];
					const uint8_t t2 = r[i+2];
					const uint8_t t3 = r[i+3];
					r[i]   = Mule[t0] ^ Mulb[t1] ^ Muld[t2] ^ Mul9[t3];
					r[i+1] = Mul9[t0] ^ Mule[t1] ^ Mulb[t2] ^ Muld[t3];
					r[i+2] = Muld[t0] ^ Mul9[t1] ^ Mule[t2] ^ Mulb[t3];
					r[i+3] = Mulb[t0] ^ Muld[t1] ^ Mul9[t2] ^ Mule[t3];
				}
				SubShiftI(r);
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
				SubShiftI(r);
				i -= 16;
				for(; i > 0; i -= 16)
				{
					v_xor(r, key.data() + i, 16);
					MixShiftI(r);
				}
				v_xor(r, key.data(), 16);
			}
		};
	};
}

#endif
