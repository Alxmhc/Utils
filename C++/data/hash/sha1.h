namespace hash
{
	class SHA1
	{
		static const uint_fast8_t hsz = 5;
		uint32_t st[hsz];
		uint64_t size;

		rbuf<64> buf;

		static void rotate(uint32_t *wt, uint32_t t)
		{
			t += rotl(wt[0], 5) + wt[4];
			wt[4] = wt[3];
			wt[3] = wt[2];
			wt[2] = rotl(wt[1], 30);
			wt[1] = wt[0];
			wt[0] = t;
		}
		void rotate1(uint32_t *wt, uint32_t t)
		{
			t += ((wt[1] & wt[2])|(~wt[1] & wt[3])) + 0x5a827999;
			rotate(wt, t);
		}
		void rotate2(uint32_t *wt, uint32_t t)
		{
			t += (wt[1]^wt[2]^wt[3]) + 0x6ed9eba1;
			rotate(wt, t);
		}
		void rotate3(uint32_t *wt, uint32_t t)
		{
			t += ((wt[1] & wt[2])|(wt[3] & (wt[1]|wt[2]))) + 0x8f1bbcdc;
			rotate(wt, t);
		}
		void rotate4(uint32_t *wt, uint32_t t)
		{
			t += (wt[1]^wt[2]^wt[3]) + 0xca62c1d6;
			rotate(wt, t);
		}

		void Transform(const std::array<uint32_t, 16> &x)
		{
			uint32_t wt[hsz];
			std::copy(st, st+hsz, wt);

			uint32_t w[80];
			for(uint_fast8_t i = 0; i < 16; i++)
			{
				w[i] = x[i];

				rotate1(wt, x[i]);
			}
			for(uint_fast8_t i = 16; i < 80; i++)
			{
				w[i] = rotl(w[i-3]^w[i-8]^w[i-14]^w[i-16], 1);

				if(i < 20)
				{
					rotate1(wt, w[i]);
				}
				else if(i < 40)
				{
					rotate2(wt, w[i]);
				}
				else if(i < 60)
				{
					rotate3(wt, w[i]);
				}
				else
				{
					rotate4(wt, w[i]);
				}
			}
			st[0] += wt[0];
			st[1] += wt[1];
			st[2] += wt[2];
			st[3] += wt[3];
			st[4] += wt[4];
		}
	public:
		void process_block(const uint8_t *v)
		{
			auto x = conv::pack4_be<buf.sz>(v);
			Transform(x);
		}

		void Clear()
		{
			size = 0;
			buf.clear();

			st[0] = 0x67452301;
			st[1] = 0xefcdab89;
			st[2] = 0x98badcfe;
			st[3] = 0x10325476;
			st[4] = 0xc3d2e1f0;
		}

		SHA1()
		{
			Clear();
		}

		void Update(const uint8_t *v, const std::size_t n)
		{
			size += n;
			buf.process(v, n, *this);
		}

		std::vector<uint8_t> Final()
		{
			buf.push(0x80);
			buf.nul();
			auto x = conv::pack4_be<buf.sz>(buf.d);
			if(buf.sz_e() < 8)
			{
				Transform(x);
				x.fill(0);
			}
			x[14] = static_cast<uint32_t>(size>>29);
			x[15] = static_cast<uint32_t>(size<<3);
			Transform(x);

			return conv::unpack1_be(st, hsz);
		}
	};
}
