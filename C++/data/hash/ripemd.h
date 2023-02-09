namespace hash
{
	class RIPEMD128 : public HASH<16, 64>
	{
		class tbf : public byteWriterBuf<block_size>
		{
			static void fn1(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t x, uint8_t s)
			{
				a += (b ^ c ^ d);
				a = rotl(a + x, s);
			}

			static void fn12(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t x, uint8_t s)
			{
				a += (d ^ (b & (c^d))) + 0x5a827999;
				a = rotl(a + x, s);
			}
			static void fn22(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t x, uint8_t s)
			{
				a += (d ^ (b & (c^d))) + 0x6d703ef3;
				a = rotl(a + x, s);
			}

			static void fn13(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t x, uint8_t s)
			{
				a += (d ^ (b | ~c)) + 0x6ed9eba1;
				a = rotl(a + x, s);
			}
			static void fn23(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t x, uint8_t s)
			{
				a += (d ^ (b | ~c)) + 0x5c4dd124;
				a = rotl(a + x, s);
			}

			static void fn14(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t x, uint8_t s)
			{
				a += (c ^ (d & (b^c))) + 0x8f1bbcdc;
				a = rotl(a + x, s);
			}
			static void fn24(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t x, uint8_t s)
			{
				a += (c ^ (d & (b^c))) + 0x50a28be6;
				a = rotl(a + x, s);
			}

			void Transform()
			{
				uint32_t a1 = st[0], b1 = st[1], c1 = st[2], d1 = st[3];
				uint32_t a2 = st[0], b2 = st[1], c2 = st[2], d2 = st[3];

				fn1(a1, b1, c1, d1, x[ 0], 11);
				fn1(d1, a1, b1, c1, x[ 1], 14);
				fn1(c1, d1, a1, b1, x[ 2], 15);
				fn1(b1, c1, d1, a1, x[ 3], 12);
				fn1(a1, b1, c1, d1, x[ 4],  5);
				fn1(d1, a1, b1, c1, x[ 5],  8);
				fn1(c1, d1, a1, b1, x[ 6],  7);
				fn1(b1, c1, d1, a1, x[ 7],  9);
				fn1(a1, b1, c1, d1, x[ 8], 11);
				fn1(d1, a1, b1, c1, x[ 9], 13);
				fn1(c1, d1, a1, b1, x[10], 14);
				fn1(b1, c1, d1, a1, x[11], 15);
				fn1(a1, b1, c1, d1, x[12],  6);
				fn1(d1, a1, b1, c1, x[13],  7);
				fn1(c1, d1, a1, b1, x[14],  9);
				fn1(b1, c1, d1, a1, x[15],  8);

				fn12(a1, b1, c1, d1, x[ 7],  7);
				fn12(d1, a1, b1, c1, x[ 4],  6);
				fn12(c1, d1, a1, b1, x[13],  8);
				fn12(b1, c1, d1, a1, x[ 1], 13);
				fn12(a1, b1, c1, d1, x[10], 11);
				fn12(d1, a1, b1, c1, x[ 6],  9);
				fn12(c1, d1, a1, b1, x[15],  7);
				fn12(b1, c1, d1, a1, x[ 3], 15);
				fn12(a1, b1, c1, d1, x[12],  7);
				fn12(d1, a1, b1, c1, x[ 0], 12);
				fn12(c1, d1, a1, b1, x[ 9], 15);
				fn12(b1, c1, d1, a1, x[ 5],  9);
				fn12(a1, b1, c1, d1, x[ 2], 11);
				fn12(d1, a1, b1, c1, x[14],  7);
				fn12(c1, d1, a1, b1, x[11], 13);
				fn12(b1, c1, d1, a1, x[ 8], 12);

				fn13(a1, b1, c1, d1, x[ 3], 11);
				fn13(d1, a1, b1, c1, x[10], 13);
				fn13(c1, d1, a1, b1, x[14],  6);
				fn13(b1, c1, d1, a1, x[ 4],  7);
				fn13(a1, b1, c1, d1, x[ 9], 14);
				fn13(d1, a1, b1, c1, x[15],  9);
				fn13(c1, d1, a1, b1, x[ 8], 13);
				fn13(b1, c1, d1, a1, x[ 1], 15);
				fn13(a1, b1, c1, d1, x[ 2], 14);
				fn13(d1, a1, b1, c1, x[ 7],  8);
				fn13(c1, d1, a1, b1, x[ 0], 13);
				fn13(b1, c1, d1, a1, x[ 6],  6);
				fn13(a1, b1, c1, d1, x[13],  5);
				fn13(d1, a1, b1, c1, x[11], 12);
				fn13(c1, d1, a1, b1, x[ 5],  7);
				fn13(b1, c1, d1, a1, x[12],  5);

				fn14(a1, b1, c1, d1, x[ 1], 11);
				fn14(d1, a1, b1, c1, x[ 9], 12);
				fn14(c1, d1, a1, b1, x[11], 14);
				fn14(b1, c1, d1, a1, x[10], 15);
				fn14(a1, b1, c1, d1, x[ 0], 14);
				fn14(d1, a1, b1, c1, x[ 8], 15);
				fn14(c1, d1, a1, b1, x[12],  9);
				fn14(b1, c1, d1, a1, x[ 4],  8);
				fn14(a1, b1, c1, d1, x[13],  9);
				fn14(d1, a1, b1, c1, x[ 3], 14);
				fn14(c1, d1, a1, b1, x[ 7],  5);
				fn14(b1, c1, d1, a1, x[15],  6);
				fn14(a1, b1, c1, d1, x[14],  8);
				fn14(d1, a1, b1, c1, x[ 5],  6);
				fn14(c1, d1, a1, b1, x[ 6],  5);
				fn14(b1, c1, d1, a1, x[ 2], 12);

				fn24(a2, b2, c2, d2, x[ 5],  8);
				fn24(d2, a2, b2, c2, x[14],  9);
				fn24(c2, d2, a2, b2, x[ 7],  9);
				fn24(b2, c2, d2, a2, x[ 0], 11);
				fn24(a2, b2, c2, d2, x[ 9], 13);
				fn24(d2, a2, b2, c2, x[ 2], 15);
				fn24(c2, d2, a2, b2, x[11], 15);
				fn24(b2, c2, d2, a2, x[ 4],  5);
				fn24(a2, b2, c2, d2, x[13],  7);
				fn24(d2, a2, b2, c2, x[ 6],  7);
				fn24(c2, d2, a2, b2, x[15],  8);
				fn24(b2, c2, d2, a2, x[ 8], 11);
				fn24(a2, b2, c2, d2, x[ 1], 14);
				fn24(d2, a2, b2, c2, x[10], 14);
				fn24(c2, d2, a2, b2, x[ 3], 12);
				fn24(b2, c2, d2, a2, x[12],  6);

				fn23(a2, b2, c2, d2, x[ 6],  9);
				fn23(d2, a2, b2, c2, x[11], 13);
				fn23(c2, d2, a2, b2, x[ 3], 15);
				fn23(b2, c2, d2, a2, x[ 7],  7);
				fn23(a2, b2, c2, d2, x[ 0], 12);
				fn23(d2, a2, b2, c2, x[13],  8);
				fn23(c2, d2, a2, b2, x[ 5],  9);
				fn23(b2, c2, d2, a2, x[10], 11);
				fn23(a2, b2, c2, d2, x[14],  7);
				fn23(d2, a2, b2, c2, x[15],  7);
				fn23(c2, d2, a2, b2, x[ 8], 12);
				fn23(b2, c2, d2, a2, x[12],  7);
				fn23(a2, b2, c2, d2, x[ 4],  6);
				fn23(d2, a2, b2, c2, x[ 9], 15);
				fn23(c2, d2, a2, b2, x[ 1], 13);
				fn23(b2, c2, d2, a2, x[ 2], 11);

				fn22(a2, b2, c2, d2, x[15],  9);
				fn22(d2, a2, b2, c2, x[ 5],  7);
				fn22(c2, d2, a2, b2, x[ 1], 15);
				fn22(b2, c2, d2, a2, x[ 3], 11);
				fn22(a2, b2, c2, d2, x[ 7],  8);
				fn22(d2, a2, b2, c2, x[14],  6);
				fn22(c2, d2, a2, b2, x[ 6],  6);
				fn22(b2, c2, d2, a2, x[ 9], 14);
				fn22(a2, b2, c2, d2, x[11], 12);
				fn22(d2, a2, b2, c2, x[ 8], 13);
				fn22(c2, d2, a2, b2, x[12],  5);
				fn22(b2, c2, d2, a2, x[ 2], 14);
				fn22(a2, b2, c2, d2, x[10], 13);
				fn22(d2, a2, b2, c2, x[ 0], 13);
				fn22(c2, d2, a2, b2, x[ 4],  7);
				fn22(b2, c2, d2, a2, x[13],  5);

				fn1(a2, b2, c2, d2, x[ 8], 15);
				fn1(d2, a2, b2, c2, x[ 6],  5);
				fn1(c2, d2, a2, b2, x[ 4],  8);
				fn1(b2, c2, d2, a2, x[ 1], 11);
				fn1(a2, b2, c2, d2, x[ 3], 14);
				fn1(d2, a2, b2, c2, x[11], 14);
				fn1(c2, d2, a2, b2, x[15],  6);
				fn1(b2, c2, d2, a2, x[ 0], 14);
				fn1(a2, b2, c2, d2, x[ 5],  6);
				fn1(d2, a2, b2, c2, x[12],  9);
				fn1(c2, d2, a2, b2, x[ 2], 12);
				fn1(b2, c2, d2, a2, x[13],  9);
				fn1(a2, b2, c2, d2, x[ 9], 12);
				fn1(d2, a2, b2, c2, x[ 7],  5);
				fn1(c2, d2, a2, b2, x[10], 15);
				fn1(b2, c2, d2, a2, x[14],  8);

				c1 += st[1] + d2;
				st[1] = st[2] + d1 + a2;
				st[2] = st[3] + a1 + b2;
				st[3] = st[0] + b1 + c2;
				st[0] = c1;
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
		RIPEMD128()
		{
			buf.Init();
		}
		void Update(const uint8_t* v, const size_t n)
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

	class RIPEMD160 : public HASH<20, 64>
	{
		class tbf : public byteWriterBuf<block_size>
		{
			static void f(uint32_t &a, uint32_t &c, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a = rotl(a + x, s) + e;
				c = rotl(c, 10);
			}

			static void fn1(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += (b ^ c ^ d);
				f(a, c, e, x, s);
			}

			static void fn12(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += (d ^ (b & (c^d))) + 0x5a827999;
				f(a, c, e, x, s);
			}
			static void fn22(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += (d ^ (b & (c^d))) + 0x7a6d76e9;
				f(a, c, e, x, s);
			}

			static void fn13(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += ((b | ~c) ^ d) + 0x6ed9eba1;
				f(a, c, e, x, s);
			}
			static void fn23(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += ((b | ~c) ^ d) + 0x6d703ef3;
				f(a, c, e, x, s);
			}

			static void fn14(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += (c ^ (d & (b^c))) + 0x8f1bbcdc;
				f(a, c, e, x, s);
			}
			static void fn24(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += (c ^ (d & (b^c))) + 0x5c4dd124;
				f(a, c, e, x, s);
			}

			static void fn15(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += (b ^ (c | ~d)) + 0xa953fd4e;
				f(a, c, e, x, s);
			}
			static void fn25(uint32_t &a, uint32_t b, uint32_t &c, uint32_t d, uint32_t e, uint32_t x, uint_fast8_t s)
			{
				a += (b ^ (c | ~d)) + 0x50a28be6;
				f(a, c, e, x, s);
			}

			void Transform()
			{
				uint32_t a1 = st[0], b1 = st[1], c1 = st[2], d1 = st[3], e1 = st[4];
				uint32_t a2 = st[0], b2 = st[1], c2 = st[2], d2 = st[3], e2 = st[4];

				fn1(a1, b1, c1, d1, e1, x[ 0], 11);
				fn1(e1, a1, b1, c1, d1, x[ 1], 14);
				fn1(d1, e1, a1, b1, c1, x[ 2], 15);
				fn1(c1, d1, e1, a1, b1, x[ 3], 12);
				fn1(b1, c1, d1, e1, a1, x[ 4],  5);
				fn1(a1, b1, c1, d1, e1, x[ 5],  8);
				fn1(e1, a1, b1, c1, d1, x[ 6],  7);
				fn1(d1, e1, a1, b1, c1, x[ 7],  9);
				fn1(c1, d1, e1, a1, b1, x[ 8], 11);
				fn1(b1, c1, d1, e1, a1, x[ 9], 13);
				fn1(a1, b1, c1, d1, e1, x[10], 14);
				fn1(e1, a1, b1, c1, d1, x[11], 15);
				fn1(d1, e1, a1, b1, c1, x[12],  6);
				fn1(c1, d1, e1, a1, b1, x[13],  7);
				fn1(b1, c1, d1, e1, a1, x[14],  9);
				fn1(a1, b1, c1, d1, e1, x[15],  8);

				fn12(e1, a1, b1, c1, d1, x[ 7],  7);
				fn12(d1, e1, a1, b1, c1, x[ 4],  6);
				fn12(c1, d1, e1, a1, b1, x[13],  8);
				fn12(b1, c1, d1, e1, a1, x[ 1], 13);
				fn12(a1, b1, c1, d1, e1, x[10], 11);
				fn12(e1, a1, b1, c1, d1, x[ 6],  9);
				fn12(d1, e1, a1, b1, c1, x[15],  7);
				fn12(c1, d1, e1, a1, b1, x[ 3], 15);
				fn12(b1, c1, d1, e1, a1, x[12],  7);
				fn12(a1, b1, c1, d1, e1, x[ 0], 12);
				fn12(e1, a1, b1, c1, d1, x[ 9], 15);
				fn12(d1, e1, a1, b1, c1, x[ 5],  9);
				fn12(c1, d1, e1, a1, b1, x[ 2], 11);
				fn12(b1, c1, d1, e1, a1, x[14],  7);
				fn12(a1, b1, c1, d1, e1, x[11], 13);
				fn12(e1, a1, b1, c1, d1, x[ 8], 12);

				fn13(d1, e1, a1, b1, c1, x[ 3], 11);
				fn13(c1, d1, e1, a1, b1, x[10], 13);
				fn13(b1, c1, d1, e1, a1, x[14],  6);
				fn13(a1, b1, c1, d1, e1, x[ 4],  7);
				fn13(e1, a1, b1, c1, d1, x[ 9], 14);
				fn13(d1, e1, a1, b1, c1, x[15],  9);
				fn13(c1, d1, e1, a1, b1, x[ 8], 13);
				fn13(b1, c1, d1, e1, a1, x[ 1], 15);
				fn13(a1, b1, c1, d1, e1, x[ 2], 14);
				fn13(e1, a1, b1, c1, d1, x[ 7],  8);
				fn13(d1, e1, a1, b1, c1, x[ 0], 13);
				fn13(c1, d1, e1, a1, b1, x[ 6],  6);
				fn13(b1, c1, d1, e1, a1, x[13],  5);
				fn13(a1, b1, c1, d1, e1, x[11], 12);
				fn13(e1, a1, b1, c1, d1, x[ 5],  7);
				fn13(d1, e1, a1, b1, c1, x[12],  5);

				fn14(c1, d1, e1, a1, b1, x[ 1], 11);
				fn14(b1, c1, d1, e1, a1, x[ 9], 12);
				fn14(a1, b1, c1, d1, e1, x[11], 14);
				fn14(e1, a1, b1, c1, d1, x[10], 15);
				fn14(d1, e1, a1, b1, c1, x[ 0], 14);
				fn14(c1, d1, e1, a1, b1, x[ 8], 15);
				fn14(b1, c1, d1, e1, a1, x[12],  9);
				fn14(a1, b1, c1, d1, e1, x[ 4],  8);
				fn14(e1, a1, b1, c1, d1, x[13],  9);
				fn14(d1, e1, a1, b1, c1, x[ 3], 14);
				fn14(c1, d1, e1, a1, b1, x[ 7],  5);
				fn14(b1, c1, d1, e1, a1, x[15],  6);
				fn14(a1, b1, c1, d1, e1, x[14],  8);
				fn14(e1, a1, b1, c1, d1, x[ 5],  6);
				fn14(d1, e1, a1, b1, c1, x[ 6],  5);
				fn14(c1, d1, e1, a1, b1, x[ 2], 12);

				fn15(b1, c1, d1, e1, a1, x[ 4],  9);
				fn15(a1, b1, c1, d1, e1, x[ 0], 15);
				fn15(e1, a1, b1, c1, d1, x[ 5],  5);
				fn15(d1, e1, a1, b1, c1, x[ 9], 11);
				fn15(c1, d1, e1, a1, b1, x[ 7],  6);
				fn15(b1, c1, d1, e1, a1, x[12],  8);
				fn15(a1, b1, c1, d1, e1, x[ 2], 13);
				fn15(e1, a1, b1, c1, d1, x[10], 12);
				fn15(d1, e1, a1, b1, c1, x[14],  5);
				fn15(c1, d1, e1, a1, b1, x[ 1], 12);
				fn15(b1, c1, d1, e1, a1, x[ 3], 13);
				fn15(a1, b1, c1, d1, e1, x[ 8], 14);
				fn15(e1, a1, b1, c1, d1, x[11], 11);
				fn15(d1, e1, a1, b1, c1, x[ 6],  8);
				fn15(c1, d1, e1, a1, b1, x[15],  5);
				fn15(b1, c1, d1, e1, a1, x[13],  6);

				fn25(a2, b2, c2, d2, e2, x[ 5],  8);
				fn25(e2, a2, b2, c2, d2, x[14],  9);
				fn25(d2, e2, a2, b2, c2, x[ 7],  9);
				fn25(c2, d2, e2, a2, b2, x[ 0], 11);
				fn25(b2, c2, d2, e2, a2, x[ 9], 13);
				fn25(a2, b2, c2, d2, e2, x[ 2], 15);
				fn25(e2, a2, b2, c2, d2, x[11], 15);
				fn25(d2, e2, a2, b2, c2, x[ 4],  5);
				fn25(c2, d2, e2, a2, b2, x[13],  7);
				fn25(b2, c2, d2, e2, a2, x[ 6],  7);
				fn25(a2, b2, c2, d2, e2, x[15],  8);
				fn25(e2, a2, b2, c2, d2, x[ 8], 11);
				fn25(d2, e2, a2, b2, c2, x[ 1], 14);
				fn25(c2, d2, e2, a2, b2, x[10], 14);
				fn25(b2, c2, d2, e2, a2, x[ 3], 12);
				fn25(a2, b2, c2, d2, e2, x[12],  6);

				fn24(e2, a2, b2, c2, d2, x[ 6],  9);
				fn24(d2, e2, a2, b2, c2, x[11], 13);
				fn24(c2, d2, e2, a2, b2, x[ 3], 15);
				fn24(b2, c2, d2, e2, a2, x[ 7],  7);
				fn24(a2, b2, c2, d2, e2, x[ 0], 12);
				fn24(e2, a2, b2, c2, d2, x[13],  8);
				fn24(d2, e2, a2, b2, c2, x[ 5],  9);
				fn24(c2, d2, e2, a2, b2, x[10], 11);
				fn24(b2, c2, d2, e2, a2, x[14],  7);
				fn24(a2, b2, c2, d2, e2, x[15],  7);
				fn24(e2, a2, b2, c2, d2, x[ 8], 12);
				fn24(d2, e2, a2, b2, c2, x[12],  7);
				fn24(c2, d2, e2, a2, b2, x[ 4],  6);
				fn24(b2, c2, d2, e2, a2, x[ 9], 15);
				fn24(a2, b2, c2, d2, e2, x[ 1], 13);
				fn24(e2, a2, b2, c2, d2, x[ 2], 11);

				fn23(d2, e2, a2, b2, c2, x[15],  9);
				fn23(c2, d2, e2, a2, b2, x[ 5],  7);
				fn23(b2, c2, d2, e2, a2, x[ 1], 15);
				fn23(a2, b2, c2, d2, e2, x[ 3], 11);
				fn23(e2, a2, b2, c2, d2, x[ 7],  8);
				fn23(d2, e2, a2, b2, c2, x[14],  6);
				fn23(c2, d2, e2, a2, b2, x[ 6],  6);
				fn23(b2, c2, d2, e2, a2, x[ 9], 14);
				fn23(a2, b2, c2, d2, e2, x[11], 12);
				fn23(e2, a2, b2, c2, d2, x[ 8], 13);
				fn23(d2, e2, a2, b2, c2, x[12],  5);
				fn23(c2, d2, e2, a2, b2, x[ 2], 14);
				fn23(b2, c2, d2, e2, a2, x[10], 13);
				fn23(a2, b2, c2, d2, e2, x[ 0], 13);
				fn23(e2, a2, b2, c2, d2, x[ 4],  7);
				fn23(d2, e2, a2, b2, c2, x[13],  5);

				fn22(c2, d2, e2, a2, b2, x[ 8], 15);
				fn22(b2, c2, d2, e2, a2, x[ 6],  5);
				fn22(a2, b2, c2, d2, e2, x[ 4],  8);
				fn22(e2, a2, b2, c2, d2, x[ 1], 11);
				fn22(d2, e2, a2, b2, c2, x[ 3], 14);
				fn22(c2, d2, e2, a2, b2, x[11], 14);
				fn22(b2, c2, d2, e2, a2, x[15],  6);
				fn22(a2, b2, c2, d2, e2, x[ 0], 14);
				fn22(e2, a2, b2, c2, d2, x[ 5],  6);
				fn22(d2, e2, a2, b2, c2, x[12],  9);
				fn22(c2, d2, e2, a2, b2, x[ 2], 12);
				fn22(b2, c2, d2, e2, a2, x[13],  9);
				fn22(a2, b2, c2, d2, e2, x[ 9], 12);
				fn22(e2, a2, b2, c2, d2, x[ 7],  5);
				fn22(d2, e2, a2, b2, c2, x[10], 15);
				fn22(c2, d2, e2, a2, b2, x[14],  8);

				fn1(b2, c2, d2, e2, a2, x[12],  8);
				fn1(a2, b2, c2, d2, e2, x[15],  5);
				fn1(e2, a2, b2, c2, d2, x[10], 12);
				fn1(d2, e2, a2, b2, c2, x[ 4],  9);
				fn1(c2, d2, e2, a2, b2, x[ 1], 12);
				fn1(b2, c2, d2, e2, a2, x[ 5],  5);
				fn1(a2, b2, c2, d2, e2, x[ 8], 14);
				fn1(e2, a2, b2, c2, d2, x[ 7],  6);
				fn1(d2, e2, a2, b2, c2, x[ 6],  8);
				fn1(c2, d2, e2, a2, b2, x[ 2], 13);
				fn1(b2, c2, d2, e2, a2, x[13],  6);
				fn1(a2, b2, c2, d2, e2, x[14],  5);
				fn1(e2, a2, b2, c2, d2, x[ 0], 15);
				fn1(d2, e2, a2, b2, c2, x[ 3], 13);
				fn1(c2, d2, e2, a2, b2, x[ 9], 11);
				fn1(b2, c2, d2, e2, a2, x[11], 11);

				c1 += st[1] + d2;
				st[1] = st[2] + d1 + e2;
				st[2] = st[3] + e1 + a2;
				st[3] = st[4] + a1 + b2;
				st[4] = st[0] + b1 + c2;
				st[0] = c1;
			}

			std::array<uint32_t, 16> x;

			void process(const uint8_t* v)
			{
				conv::pack<4, endianness::LITTLE_ENDIAN>(v, bsize, x.data());
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
		RIPEMD160()
		{
			buf.Init();
		}
		void Update(const uint8_t* v, const size_t n)
		{
			buf.writeN(v, n);
			buf.sz += n;
		}
		void Final(uint8_t* r)
		{
			buf.Fin();
			conv::unpack<4, endianness::LITTLE_ENDIAN>(buf.st, 5, r);
			buf.Init();
		}
	};
}
