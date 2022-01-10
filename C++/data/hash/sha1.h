namespace hash
{
	class SHA1
	{
	public:
		static const uint_fast8_t hash_size = 20;
		static const uint_fast8_t block_size = 64;
	private:
		uint64_t size;
		uint32_t st[hash_size >> 2];
		rbuf<block_size> buf;
		uint32_t x[block_size >> 2];

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
			st[0] += wt[0];
			st[1] += wt[1];
			st[2] += wt[2];
			st[3] += wt[3];
			st[4] += wt[4];
		}

	public:
		void process_block(const uint8_t *v)
		{
			conv::pack<endianness::BIG_ENDIAN>(v, buf.sz, x);
			Transform();
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
		void Update(const uint8_t *v, const size_t n)
		{
			buf.process(v, n, *this);
			size += n;
		}
		void Final(uint8_t *r)
		{
			buf.push(0x80, *this);
			if(buf.size() != 0)
			{
				buf.nul();
				conv::pack<endianness::BIG_ENDIAN>(buf.data(), buf.sz, x);
				if(buf.sz_e() < 8)
				{
					Transform();
					memset(x, 0, sizeof(x));
				}
			}
			else
			{
				memset(x, 0, sizeof(x));
			}
			buf.clear();
			x[14] = static_cast<uint32_t>(size>>29);
			x[15] = static_cast<uint32_t>(size<<3);
			Transform();
			memset(x, 0, sizeof(x));
			conv::unpack<endianness::BIG_ENDIAN>(st, 5, r);
		}
	};
}
