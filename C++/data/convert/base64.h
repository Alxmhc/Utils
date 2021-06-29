namespace convert
{
	namespace base64
	{
		const std::string dct_std = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		class Encoder
		{
			std::string en;
			byteWriter &bw;
			rbuf<3> buf;
		public:
			Encoder(const std::string &b, byteWriter &w) : en(b), bw(w) {}

			void process_block(const uint8_t *v)
			{
				uint_fast32_t t = (v[0] << 16) | (v[1] << 8) | v[2];
				uint8_t r[4];
				r[0] = en[(t>>18) & 0x3f];
				r[1] = en[(t>>12) & 0x3f];
				r[2] = en[(t>>6) & 0x3f];
				r[3] = en[t & 0x3f];
				bw.write(r, 4);
			}

			void Update(const uint8_t *v, const size_t n)
			{
				buf.process(v, n, *this);
			}

			void Final()
			{
				auto d = buf.size();
				if(d == 0)
					return;
				auto v = buf.data();
				uint8_t c[3];
				c[0] = en[v[0]>>2];
				if(d == 1)
				{
					c[1] = en[(v[0] & 0x03) << 4];
					bw.write(c, 2);
				}
				else
				{
					c[1] = en[((v[0] & 0x03) << 4) + (v[1] >> 4)];
					c[2] = en[(v[1] & 0x0f) << 2];
					bw.write(c, 3);
				}
			}
		};

		class Decoder
		{
			std::array<uint8_t, 256> de;
			byteWriter &bw;
			rbuf<4> buf;
		public:
			void process_block(const uint8_t *v)
			{
				uint8_t c1 = de[v[0]];
				uint8_t c2 = de[v[1]];
				uint8_t c3 = de[v[2]];
				uint8_t c4 = de[v[3]];
				uint8_t r[3];
				r[0] = (c1 << 2) + ((c2 & 0x30) >> 4);
				r[1] = ((c2 & 0x0f) << 4) + ((c3 & 0x3c) >> 2);
				r[2] = ((c3 & 0x03) << 6) + c4;
				bw.write(r, 3);
			}

			Decoder(const std::string &b, byteWriter &w) : bw(w)
			{
				de.fill(-1);
				uint8_t sz = static_cast<uint8_t>(b.size());
				while(sz != 0)
				{
					sz--;
					de[b[sz]] = sz;
				}
			}

			void Update(const uint8_t *v, const size_t n)
			{
				buf.process(v, n, *this);
			}

			void Final()
			{
				auto d = buf.size();
				if(d < 2)
					return;

				auto v = buf.data();
				const uint8_t c1 = de[v[0]];
				const uint8_t c2 = de[v[1]];
				uint8_t c = (c1 << 2) + ((c2 & 0x30) >> 4);
				bw.write(&c, 1);
				if(d == 3)
				{
					const uint8_t c3 = de[v[2]];
					c = ((c2 & 0xf) << 4) + ((c3 & 0x3c) >> 2);
					bw.write(&c, 1);
				}
			}
		};
	}
}
