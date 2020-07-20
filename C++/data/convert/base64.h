namespace convert
{
	namespace base64
	{
		const std::string dct_std = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		class Encoder
		{
			std::string en;
		public:
			Encoder(const std::string &b) : en(b) {}

			std::vector<uint8_t> Convert(const uint8_t *v, const std::size_t n) const
			{
				std::vector<uint8_t> out;
				out.reserve(((n/3)<<2)+3);

				std::size_t k = 0;
				for(; k + 2 < n; k += 3)
				{
					uint_fast32_t t = (v[k] << 16) | (v[k+1] << 8) | v[k+2];
					out.push_back( en[(t>>18) & 0x3f] );
					out.push_back( en[(t>>12) & 0x3f] );
					out.push_back( en[(t>>6) & 0x3f] );
					out.push_back( en[t & 0x3f] );
				}
				auto d = n - k;
				if(d != 0)
				{
					out.push_back( en[v[k]>>2]);
					if(d == 1)
					{
						out.push_back( en[(v[k] & 0x03) << 4] );
					}
					else
					{
						out.push_back( en[((v[k] & 0x03) << 4) + (v[1] >> 4)] );
						out.push_back( en[(v[k+1] & 0x0f) << 2] );
					}
				}
				return out;
			}
		};

		class Decoder
		{
			std::array<uint8_t, 256> de;
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

			std::vector<uint8_t> Convert(const uint8_t *v, const std::size_t n) const
			{
				std::vector<uint8_t> out;
				out.reserve(3*(n>>2)+2);

				std::size_t k = 0;
				for(; k + 3 < n; k += 4)
				{
					uint8_t c1 = de[v[k]];
					uint8_t c2 = de[v[k+1]];
					uint8_t c3 = de[v[k+2]];
					uint8_t c4 = de[v[k+3]];
					out.push_back( (c1 << 2) + ((c2 & 0x30) >> 4) );
					out.push_back( ((c2 & 0x0f) << 4) + ((c3 & 0x3c) >> 2) );
					out.push_back( ((c3 & 0x03) << 6) + c4 );
				}
				auto d = n - k;
				if(d != 0)
				{
					uint8_t c1 = de[v[k]];
					uint8_t c2 = de[v[k+1]];
					out.push_back( (c1 << 2) + ((c2 & 0x30) >> 4) );
					if(d == 3)
					{
						uint8_t c3 = de[v[k+2]];
						out.push_back( ((c2 & 0xf) << 4) + ((c3 & 0x3c) >> 2) );
					}
				}
				return out;
			}
		};
	}
}
