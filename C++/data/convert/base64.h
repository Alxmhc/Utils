namespace conv
{
	const std::string base64_std = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	struct base64
	{
		class Encoder
		{
			std::string out;
			std::string en;
			rbuf<3> buf;
		public:
			Encoder(const std::string &b) : en(b) {}

			std::string Convert(const uint8_t *v, const std::size_t n)
			{
				Update(v, n);
				return Final();
			}

			void process_block(const uint8_t *v)
			{
				uint_fast32_t t = (v[0] << 16) | (v[1] << 8) | v[2];
				out.push_back( en[(t>>18) & 0x3f] );
				out.push_back( en[(t>>12) & 0x3f] );
				out.push_back( en[(t>>6) & 0x3f] );
				out.push_back( en[t & 0x3f] );
			}

			void Update(const uint8_t *v, const std::size_t n)
			{
				buf.process(v, n, *this);
			}

			std::string Read()
			{
				std::string res;
				std::swap(out, res);
				return res;
			}

			std::string Final()
			{
				std::size_t sz = buf.size();
				if(sz != 0)
				{
					auto v = buf.data();
					out.push_back( en[v[0]>>2]);
					if(sz == 1)
					{
						out.push_back( en[(v[0] & 0x03) << 4] );
					}
					else
					{
						out.push_back( en[((v[0] & 0x03) << 4) + (v[1] >> 4)] );
						out.push_back( en[(v[1] & 0x0f) << 2] );
					}
					buf.clear();
				}
				return Read();
			}
		};

		class Decoder
		{
			std::vector<uint8_t> out;
			std::array<uint8_t, 256> de;
			rbuf<4> buf;
		public:
			Decoder(const std::string &b)
			{
				de.fill(-1);
				uint8_t sz = static_cast<uint8_t>(b.size());
				while(sz != 0)
				{
					sz--;
					de[b[sz]] = sz;
				}
			}

			std::vector<uint8_t> Convert(const uint8_t *v, const std::size_t n)
			{
				Update(v, n);
				return Final();
			}

			void process_block(const uint8_t *v)
			{
				uint8_t c1 = de[v[0]];
				uint8_t c2 = de[v[1]];
				uint8_t c3 = de[v[2]];
				uint8_t c4 = de[v[3]];
				out.push_back( (c1 << 2) + ((c2 & 0x30) >> 4) );
				out.push_back( ((c2 & 0x0f) << 4) + ((c3 & 0x3c) >> 2) );
				out.push_back( ((c3 & 0x03) << 6) + c4 );
			}

			void Update(const uint8_t *v, const std::size_t n)
			{
				buf.process(v, n, *this);
			}

			std::vector<uint8_t> Read()
			{
				std::vector<uint8_t> res;
				std::swap(out, res);
				return res;
			}

			std::vector<uint8_t> Final()
			{
				std::size_t sz = buf.size();
				if(sz != 0)
				{
					auto v = buf.data();
					uint8_t c1 = de[v[0]];
					uint8_t c2 = de[v[1]];
					out.push_back( (c1 << 2) + ((c2 & 0x30) >> 4) );
					if(sz == 3)
					{
						uint8_t c3 = de[v[2]];
						out.push_back( ((c2 & 0xf) << 4) + ((c3 & 0x3c) >> 2) );
					}
					buf.clear();
				}
				return Read();
			}
		};
	};
}
