#ifndef H_SALSA
#define H_SALSA

#include "../../arr.h"
#include "../../math/base/math_.h"
#include "../pack.h"

namespace crypt
{
	class Salsa20
	{
		static void qrnd(uint32_t &x0, uint32_t &x1, uint32_t &x2, uint32_t &x3)
		{
			x1 ^= rotl(x0 + x3, 7);
			x2 ^= rotl(x1 + x0, 9);
			x3 ^= rotl(x2 + x1, 13);
			x0 ^= rotl(x3 + x2, 18);
		}
	public:
		class Enc : public byteProcBuf<64>
		{
			uint32_t key[16];
			const uint_fast8_t rounds;

			void gen()
			{
				uint32_t tmp[16];
				std::copy_n(key, 16, tmp);
				for(uint_fast8_t i = rounds; i != 0; i-=2)
				{
					qrnd( tmp[0],  tmp[4],  tmp[8], tmp[12]);
					qrnd( tmp[5],  tmp[9], tmp[13],  tmp[1]);
					qrnd(tmp[10], tmp[14],  tmp[2],  tmp[6]);
					qrnd(tmp[15],  tmp[3],  tmp[7], tmp[11]);

					qrnd( tmp[0],  tmp[1],  tmp[2],  tmp[3]);
					qrnd( tmp[5],  tmp[6],  tmp[7],  tmp[4]);
					qrnd(tmp[10], tmp[11],  tmp[8],  tmp[9]);
					qrnd(tmp[15], tmp[12], tmp[13], tmp[14]);
				}
				v_add(tmp, key, 16);
				conv::unpack<4, endianness::LITTLE_ENDIAN>(tmp, 16, buf);

				key[8]++;
				if(key[8] == 0)
				{
					key[9]++;
				}
			}
		public:
			Enc(const uint8_t *k, size_t length, const uint8_t* iv, uint_fast8_t r = 20) : rounds(r)
			{
				key[0] = 0x61707865;
				key[15] = 0x6b206574;
				conv::pack<4, endianness::LITTLE_ENDIAN>(k, 16, key + 1);
				if(length == 16)
				{
					key[5] = 0x3120646e;
					key[10] = 0x79622d36;
					std::copy_n(key + 1, 4, key + 11);
				}
				else
				{
					key[5] = 0x3320646e;
					key[10] = 0x79622d32;
					conv::pack<4, endianness::LITTLE_ENDIAN>(k + 16, 16, key + 11);
				}
				key[6] = bconv<1, 4, endianness::LITTLE_ENDIAN>::pack(iv);
				key[7] = bconv<1, 4, endianness::LITTLE_ENDIAN>::pack(iv + 4);
				key[8] = 0;
				key[9] = 0;
			}

			void set_num(uint64_t n)
			{
				key[8] = static_cast<uint32_t>(n);
				key[9] = n >> 32;
			}
		};

		typedef Enc Dec;
	};
}

#endif
