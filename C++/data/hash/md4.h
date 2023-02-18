#ifndef H_MD4
#define H_MD4

namespace hash
{
	class MD4 : public HASH<16, 64>
	{
		class tbf : public byteWriterBuf<block_size>
		{
			static void FF(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint_fast8_t s)
			{
				uint32_t t = (b&c)|(~b&d);
				a = rotl(a + t + x, s);
			}
			static void GG(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint_fast8_t s)
			{
				uint32_t t = (b&c)|(b&d)|(c&d);
				a = rotl(a + t + x + 0x5a827999, s);
			}
			static void HH(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint_fast8_t s)
			{
				uint32_t t = b^c^d;
				a = rotl(a + t + x + 0x6ed9eba1, s);
			}

			void Transform()
			{
				uint32_t a=st[0], b=st[1], c=st[2], d=st[3];

				FF(a, b, c, d, x[ 0],  3);
				FF(d, a, b, c, x[ 1],  7);
				FF(c, d, a, b, x[ 2], 11);
				FF(b, c, d, a, x[ 3], 19);
				FF(a, b, c, d, x[ 4],  3);
				FF(d, a, b, c, x[ 5],  7);
				FF(c, d, a, b, x[ 6], 11);
				FF(b, c, d, a, x[ 7], 19);
				FF(a, b, c, d, x[ 8],  3);
				FF(d, a, b, c, x[ 9],  7);
				FF(c, d, a, b, x[10], 11);
				FF(b, c, d, a, x[11], 19);
				FF(a, b, c, d, x[12],  3);
				FF(d, a, b, c, x[13],  7);
				FF(c, d, a, b, x[14], 11);
				FF(b, c, d, a, x[15], 19);

				GG(a, b, c, d, x[ 0],  3);
				GG(d, a, b, c, x[ 4],  5);
				GG(c, d, a, b, x[ 8],  9);
				GG(b, c, d, a, x[12], 13);
				GG(a, b, c, d, x[ 1],  3);
				GG(d, a, b, c, x[ 5],  5);
				GG(c, d, a, b, x[ 9],  9);
				GG(b, c, d, a, x[13], 13);
				GG(a, b, c, d, x[ 2],  3);
				GG(d, a, b, c, x[ 6],  5);
				GG(c, d, a, b, x[10],  9);
				GG(b, c, d, a, x[14], 13);
				GG(a, b, c, d, x[ 3],  3);
				GG(d, a, b, c, x[ 7],  5);
				GG(c, d, a, b, x[11],  9);
				GG(b, c, d, a, x[15], 13);

				HH(a, b, c, d, x[ 0],  3);
				HH(d, a, b, c, x[ 8],  9);
				HH(c, d, a, b, x[ 4], 11);
				HH(b, c, d, a, x[12], 15);
				HH(a, b, c, d, x[ 2],  3);
				HH(d, a, b, c, x[10],  9);
				HH(c, d, a, b, x[ 6], 11);
				HH(b, c, d, a, x[14], 15);
				HH(a, b, c, d, x[ 1],  3);
				HH(d, a, b, c, x[ 9],  9);
				HH(c, d, a, b, x[ 5], 11);
				HH(b, c, d, a, x[13], 15);
				HH(a, b, c, d, x[ 3],  3);
				HH(d, a, b, c, x[11],  9);
				HH(c, d, a, b, x[ 7], 11);
				HH(b, c, d, a, x[15], 15);

				st[0] += a;
				st[1] += b;
				st[2] += c;
				st[3] += d;
			}

			std::array<uint32_t, 16> x;

			void process(const uint8_t* v)
			{
				conv::pack<4, endianness::LITTLE_ENDIAN>(v, bsize, x.data());
				Transform();
			}
		public:
			uint32_t st[4];
			uint_fast64_t sz;

			void Init()
			{
				st[0] = 0x67452301;
				st[1] = 0xefcdab89;
				st[2] = 0x98badcfe;
				st[3] = 0x10325476;
				sz = 0;
			}
			void Fin()
			{
				write(0x80);
				if(size() != 0)
				{
					fill_e(0);
					conv::pack<4, endianness::LITTLE_ENDIAN>(data(), bsize, x.data());
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
				x[14] = static_cast<uint32_t>(sz<<3);
				x[15] = static_cast<uint32_t>(sz>>29);
				Transform();
				x.fill(0);
			}
		};
		tbf buf;
	public:
		MD4()
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
			conv::unpack<4, endianness::LITTLE_ENDIAN>(buf.st, 4, r);
			buf.Init();
		}
	};
}

#endif
