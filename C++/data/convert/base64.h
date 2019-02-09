namespace conv
{
	const std::string base64_std = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	class base64
	{
		std::string en;
		std::array<uint8_t, 256> de;

		void init_de()
		{
			de.fill(-1);
			uint8_t sz = static_cast<uint8_t>(en.size());
			while(sz--)
			{
				de[en[sz]] = sz;
			}
		}
	public:
		base64(const std::string &b)
		{
			reinit(b);
		}

		void reinit(const std::string &b)
		{
			en = b;
			init_de();
		}

		std::string encode(const uint8_t *v, std::size_t n) const
		{
			std::string r;
			r.reserve(n + (n >> 1));

			std::size_t j = 0;
			for(; j + 3 <= n; j += 3)
			{
				uint_fast32_t t = (v[j] << 16) | (v[j + 1] << 8) | v[j + 2];
				r.push_back(en[(t>>18) & 0x3f]);
				r.push_back(en[(t>>12) & 0x3f]);
				r.push_back(en[(t>>6) & 0x3f]);
				r.push_back(en[t & 0x3f]);
			}

			std::size_t d = n - j;
			if(d == 0)
				return r;
			r.push_back(en[(v[j]>>2) & 0x3f]);
			if(d == 1)
			{
				r.push_back(en[(v[j] & 0x03) << 4]);
			}
			else
			{
				r.push_back(en[((v[j] & 0x03) << 4) + (v[j + 1] >> 4)]);
				r.push_back(en[(v[j + 1] & 0x0f) << 2]);
			}
			return r;
		}

		std::vector<uint8_t> decode(const std::string &s) const
		{
			std::size_t n = s.length();
			if(n == 0)
				return std::vector<uint8_t>();
			std::vector<uint8_t> r;
			r.reserve(3*(n>>2) + (n&3));

			uint_fast8_t c[4];
			std::size_t j = 0;
			for(; j + 4 <= n; j += 4)
			{
				c[0] = de[s[j]];
				c[1] = de[s[j+1]];
				c[2] = de[s[j+2]];
				c[3] = de[s[j+3]];

				r.push_back( (c[0] << 2) + ((c[1] & 0x30) >> 4) );
				r.push_back( ((c[1] & 0x0f) << 4) + ((c[2] & 0x3c) >> 2) );
				r.push_back( ((c[2] & 0x03) << 6) + c[3] );
			}

			std::size_t d = n - j;
			if(d == 0)
				return r;
			c[0] = de[s[j]];
			c[1] = de[s[j+1]];
			r.push_back( (c[0] << 2) + ((c[1] & 0x30) >> 4) );
			if(d == 3)
			{
				c[2] = de[s[j+2]];
				r.push_back( ((c[1] & 0xf) << 4) + ((c[2] & 0x3c) >> 2) );
			}
			return r;
		}
	};
}
