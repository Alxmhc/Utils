#ifndef H_SM3
#define H_SM3

#include <array>

#include "../../math/base/math_.h"
#include "../byte_writer.h"

namespace hash
{
	class SM3
	{
	public:
		static const uint_fast8_t hash_size = 32;
		static const uint_fast8_t block_size = 64;
	private:
		class tbf : public byteWriterBuf<block_size>
		{
			void Transform()
			{
				uint32_t W[68];
				std::copy_n(x.data(), x.size(), W);
				for(uint_fast8_t i = 16; i < 68; i++)
				{
					uint32_t tmp = W[i-16] ^ W[i-9] ^ rotl(W[i-3], 15);
					W[i] = tmp ^ rotl(tmp, 15) ^ rotl(tmp, 23) ^ rotl(W[i-13], 7) ^ W[i-6];
				}
				uint32_t tmp[8];
				std::copy_n(st, 8, tmp);
				for(uint_fast8_t i = 0; i < 64; i++)
				{
					uint32_t T1 = rotl(st[0], 12) + st[4];
					uint32_t T2;
					if(i < 16)
					{
						T1 = rotl((T1 + rotl(0x79cc4519, i)), 7);
						T2 = st[4] ^ st[5] ^ st[6];
						T2 += T1;

						T1 ^= rotl(st[0], 12);
						T1 += st[0] ^ st[1] ^ st[2];
					}
					else
					{
						T1 = rotl((T1 + rotl(0x7a879d8a, i % 32)), 7);
						T2 = (st[4]&st[5])|(~st[4]&st[6]);
						T2 += T1;

						T1 ^= rotl(st[0], 12);
						T1 += (st[0]&st[1])|(st[1]&st[2])|(st[0]&st[2]);
					}
					T1 += st[3] + (W[i] ^ W[i+4]);
					T2 += st[7] + W[i];

					st[7] = st[6];
					st[6] = rotl(st[5], 19);
					st[5] = st[4];
					st[4] = T2^rotl(T2, 9)^rotl(T2, 17);
					st[3] = st[2];
					st[2] = rotl(st[1], 9);
					st[1] = st[0];
					st[0] = T1;
				}
				v_xor(st, tmp, 8);
			}

			std::array<uint32_t, 16> x;

			void process(const uint8_t* v)
			{
				conv::pack<4, endianness::BIG_ENDIAN>(v, bsize, x.data());
				Transform();
			}
		public:
			uint32_t st[8];
			uint_fast64_t sz;

			void Init()
			{
				st[0] = 0x7380166f;
				st[1] = 0x4914b2b9;
				st[2] = 0x172442d7;
				st[3] = 0xda8a0600;
				st[4] = 0xa96f30bc;
				st[5] = 0x163138aa;
				st[6] = 0xe38dee4d;
				st[7] = 0xb0fb0e4e;
				sz = 0;
			}
			void Fin()
			{
				write(0x80);
				if(size() != 0)
				{
					fill_e(0);
					conv::pack<4, endianness::BIG_ENDIAN>(data(), bsize, x.data());
					if(bsize - size() < 8)
					{
						Transform();
						x.fill(0);
					}
					reset();
				}
				else
				{
					x.fill(0);
				}
				x[14] = static_cast<uint32_t>(sz>>29);
				x[15] = static_cast<uint32_t>(sz<<3);
				Transform();
				x.fill(0);
			}
		};
		tbf buf;
	public:
		SM3()
		{
			buf.Init();
		}
		void Update(const uint8_t* v, const std::size_t n)
		{
			buf.writeN(v, n);
			buf.sz += n;
		}
		void Final(uint8_t* r)
		{
			buf.Fin();
			conv::unpack<4, endianness::BIG_ENDIAN>(buf.st, 8, r);
			buf.Init();
		}
	};
}

#endif
