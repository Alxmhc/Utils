#ifndef H_SHA1
#define H_SHA1

#include "../../arr.h"
#include "../../math/base/math_.h"
#include "../byte_writer.h"

namespace hash
{
	class SHA1
	{
	public:
		static const uint_fast8_t hash_size = 20;
		static const uint_fast8_t block_size = 64;
	private:
		class tbf : public byteWriterBuf<block_size>
		{
			void Transform()
			{
				uint32_t wt[5];
				std::copy(st, st+5, wt);

				for(uint_fast8_t i = 0; i < 80; i++)
				{
					const uint_fast8_t j = i & 0x0f;
					if(i > 15)
					{
						x[j] = rotl(x[(j+13)&0x0f]^x[(j+8)&0x0f]^x[(j+2)&0x0f]^x[j], 1);
					}

					uint32_t t = x[j] + rotl(wt[0], 5) + wt[4];
					if(i < 20)
					{
						t += ((wt[1] & wt[2])|(~wt[1] & wt[3])) + 0x5a827999;
					}
					else if(i < 40)
					{
						t += (wt[1]^wt[2]^wt[3]) + 0x6ed9eba1;
					}
					else if(i < 60)
					{
						t += ((wt[1] & wt[2])|(wt[3] & (wt[1]|wt[2]))) + 0x8f1bbcdc;
					}
					else
					{
						t += (wt[1]^wt[2]^wt[3]) + 0xca62c1d6;
					}
					wt[4] = wt[3];
					wt[3] = wt[2];
					wt[2] = rotl(wt[1], 30);
					wt[1] = wt[0];
					wt[0] = t;
				}
				v_add(st, wt, 5);
			}

			uint32_t x[16];

			void process(const uint8_t* v)
			{
				conv::pack<4, endianness::BIG_ENDIAN>(v, bsize, x);
				Transform();
			}
		public:
			uint32_t st[5];
			uint_fast64_t sz;

			void Init()
			{
				st[0] = 0x67452301;
				st[1] = 0xefcdab89;
				st[2] = 0x98badcfe;
				st[3] = 0x10325476;
				st[4] = 0xc3d2e1f0;
				sz = 0;
			}
			void Fin()
			{
				write(0x80);
				if(size() != 0)
				{
					fill_e(0);
					conv::pack<4, endianness::BIG_ENDIAN>(data(), bsize, x);
					if(bsize - size() < 8)
					{
						Transform();
						std::fill_n(x, 16, 0);
					}
					reset();
				}
				else
				{
					std::fill_n(x, 16, 0);
				}
				x[14] = static_cast<uint32_t>(sz>>29);
				x[15] = static_cast<uint32_t>(sz<<3);
				Transform();
				std::fill_n(x, 16, 0);
			}
		};
		tbf buf;
	public:
		SHA1()
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
			conv::unpack<4, endianness::BIG_ENDIAN>(buf.st, 5, r);
			buf.Init();
		}
	};
}

#endif
