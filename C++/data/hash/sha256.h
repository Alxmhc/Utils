namespace hash
{
	class SHA256
	{
		uint32_t st[8];
		uint64_t size;

		rbuf<64> buf;
		std::array<uint32_t, 16> x;

		static const uint32_t K[64];

		void Transform()
		{
			uint32_t wt[8];
			std::copy(st, st+8, wt);

			for(uint_fast8_t i = 0; i < 64; i++)
			{
				uint_fast8_t j = i & 0x0f;
				if(i > 15)
				{
					x[j] += x[(j+9)&0x0f];
					uint32_t tmp = x[(j+1)&0x0f];
					x[j] += rotr(tmp, 7) ^ rotr(tmp, 18) ^ (tmp>>3);
					tmp = x[(j+14)&0x0f];
					x[j] += rotr(tmp, 17) ^ rotr(tmp, 19) ^ (tmp>>10);
				}
				uint32_t t = x[j] + K[i] + wt[7];
				t += rotr(wt[4], 6) ^ rotr(wt[4], 11) ^ rotr(wt[4], 25);
				t += (wt[4] & wt[5]) ^ (~wt[4] & wt[6]);
				uint32_t tmp = rotr(wt[0], 2) ^ rotr(wt[0], 13) ^ rotr(wt[0], 22);
				tmp += (wt[0] & wt[1]) | (wt[2] & (wt[0]|wt[1]));
				wt[7] = wt[6];
				wt[6] = wt[5];
				wt[5] = wt[4];
				wt[4] = wt[3] + t;
				wt[3] = wt[2];
				wt[2] = wt[1];
				wt[1] = wt[0];
				wt[0] = t + tmp;
			}
			st[0] += wt[0];
			st[1] += wt[1];
			st[2] += wt[2];
			st[3] += wt[3];
			st[4] += wt[4];
			st[5] += wt[5];
			st[6] += wt[6];
			st[7] += wt[7];
		}

		void Init()
		{
			st[0] = 0x6a09e667;
			st[1] = 0xbb67ae85;
			st[2] = 0x3c6ef372;
			st[3] = 0xa54ff53a;
			st[4] = 0x510e527f;
			st[5] = 0x9b05688c;
			st[6] = 0x1f83d9ab;
			st[7] = 0x5be0cd19;

			size = 0;
			buf.init();
		}

		void Clear()
		{
			buf.clear();
			x.fill(0);
		}
	public:
		static const uint_fast16_t hash_size = 32;

		void process_block(const uint8_t *v)
		{
			conv::pack_be<buf.sz>(v, x);
			Transform();
		}

		SHA256()
		{
			Init();
		}

		void Update(const uint8_t *v, const std::size_t n)
		{
			size += n;
			buf.process(v, n, *this);
		}

		void Final(std::array<uint8_t, hash_size> &r)
		{
			buf.push(0x80, *this);
			if(buf.size() != 0)
			{
				buf.nul();
				conv::pack_be<buf.sz>(buf.data(), x);
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
			conv::unpack_be<8>(st, r);

			Clear();
			Init();
		}
	};

	const uint32_t SHA256::K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
}
