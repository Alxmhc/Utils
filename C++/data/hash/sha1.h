namespace hash
{
	class SHA1
	{
		uint32_t st[5];
		uint64_t size;

		rbuf<64> buf;
		std::array<uint32_t, 16> x;

		void Transform()
		{
			uint32_t wt[5];
			std::copy(st, st+5, wt);

			for(uint_fast8_t i = 0; i < 80; i++)
			{
				uint_fast8_t j = i & 0x0f;
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
			st[0] += wt[0];
			st[1] += wt[1];
			st[2] += wt[2];
			st[3] += wt[3];
			st[4] += wt[4];
		}

		void Init()
		{
			st[0] = 0x67452301;
			st[1] = 0xefcdab89;
			st[2] = 0x98badcfe;
			st[3] = 0x10325476;
			st[4] = 0xc3d2e1f0;

			size = 0;
		}

		void Clear()
		{
			buf.clear();
			x.fill(0);
		}
	public:
		static const uint_fast16_t hash_size = 20;

		void process_block(const uint8_t *v)
		{
			conv::pack<buf.sz, endianness::BIG_ENDIAN>(v, x);
			Transform();
		}

		SHA1()
		{
			Init();
		}

		void Update(const uint8_t *v, const std::size_t n)
		{
			size += n;
			buf.process(v, n, *this);
		}
		void Update(std::istream &s)
		{
			size += buf.process(s, *this);
		}

		void Final(std::array<uint8_t, hash_size> &r)
		{
			buf.push(0x80, *this);
			if(buf.size() != 0)
			{
				buf.nul();
				conv::pack<buf.sz, endianness::BIG_ENDIAN>(buf.data(), x);
				if(buf.sz_e() < 8)
				{
					Transform();
					x.fill(0);
				}
			}
			else
			{
				x.fill(0);
			}
			x[14] = static_cast<uint32_t>(size>>29);
			x[15] = static_cast<uint32_t>(size<<3);
			Transform();
			conv::unpack_be<5>(st, r);

			Clear();
			Init();
		}
	};
}
