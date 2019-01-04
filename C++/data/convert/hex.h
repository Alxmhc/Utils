namespace conv
{
	namespace hex
	{
		const char str_[] = "0123456789abcdef";

		std::string encode(const uint8_t *v, const std::size_t n)
		{
			std::string r;
			r.reserve(n*2);
			std::for_each(v, v + n, [&r](uint8_t c){r.push_back(str_[c>>4]); r.push_back(str_[c&0x0f]);});
			return r;
		}

		std::vector<uint8_t> decode(const std::string &s)
		{
			auto n = s.size()>>1;
			std::vector<uint8_t> r(n);
			size_t p = 0;
			char t[3] = {};
			for(size_t i=0; i<n; i++)
			{
				t[0] = s[p]; p++;
				t[1] = s[p]; p++;
				r[i] = static_cast<uint8_t>(strtoul(t, nullptr, 16));
			}
			return r;
		}
	}
}
