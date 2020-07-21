namespace decode
{
	void unchunk(const uint8_t *v, const std::size_t n, byteWriter &bw)
	{
		size_t i = 0;
		while(i < n)
		{
			auto sz = strtoul(reinterpret_cast<const char*>(v + i), nullptr, 16);
			if(sz == 0)
				break;
			while(v[i] != '\r' && i < n)
			{
				i++;
			}
			i += 2;
			if(i + sz >= n)
			{
				bw.write(v + i, n - i);
				break;
			}
			bw.write(v + i, sz);
			i += sz + 2;
		}
	}

	std::vector<uint8_t> URL(const std::string &s)
	{
		auto n = s.size();
		std::vector<uint8_t> r;
		r.reserve(n);
		char t[3] = {};
		for(size_t i = 0; i < n;)
		{
			if(s[i] != '%')
			{
				r.push_back(s[i]);
				i++;
				continue;
			}
			t[0] = s[i+1];
			t[1] = s[i+2];
			i += 3;
			uint8_t c = static_cast<uint8_t>(strtoul(t, nullptr, 16));
			r.push_back(c);
		}
		r.shrink_to_fit();
		return r;
	}
}
