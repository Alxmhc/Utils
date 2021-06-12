namespace convert
{
	namespace hex
	{
		const char *dct_l = "0123456789abcdef";
		const char *dct_u = "0123456789ABCDEF";

		std::string Encode(const uint8_t *v, const size_t n, const char *d)
		{
			std::string out;
			out.reserve(n<<1);

			for(size_t i = 0; i < n; i++)
			{
				out.push_back(d[v[i]>>4]);
				out.push_back(d[v[i]&0x0f]);
			}
			return out;
		}

		std::vector<uint8_t> Decode(const char *s, const size_t n)
		{
			std::vector<uint8_t> out;
			out.reserve(n>>1);

			char t[3] = {};
			for(size_t i = 0; i + 1 < n; i += 2)
			{
				t[0] = s[i];
				t[1] = s[i+1];
				out.push_back( static_cast<uint8_t>(strtoul(t, nullptr, 16)) );
			}
			return out;
		}
	}
}
