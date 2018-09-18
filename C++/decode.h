namespace decode
{
	void unchunk(std::vector<uint8_t> &v)
	{
		auto n = v.size();
		std::vector<uint8_t> r;
		r.reserve(n);
		for(size_t i=0, j=0; i<n; )
		{
			if(v[i] != '\r'){i++; continue;}
			v[i] = 0;
			const char *c = reinterpret_cast<char*>(v.data()) + j;
			auto sz = strtoul(c, nullptr, 16);
			if(!sz){break;}
			i += 2;
			if(i + sz >= n)
			{
				r.insert(r.end(), v.cbegin() + i, v.cend());
				break;
			}
			r.insert(r.end(), v.cbegin() + i, v.cbegin() + i + sz);
			i += sz + 2;
			j = i;
		}
		v = r;
		v.shrink_to_fit();
	}

	std::vector<uint8_t> URL(const std::string &s)
	{
		auto n = s.size();
		std::vector<uint8_t> r;
		r.reserve(n);
		char t[3] = {};
		size_t i = 0;
		while(i < n)
		{
			if(s[i] != '%'){r.push_back(s[i++]); continue;}
			i++;
			t[0] = s[i++]; t[1] = s[i++];
			uint8_t c = static_cast<uint8_t>(strtoul(t, nullptr, 16));
			r.push_back(c);
		}
		r.shrink_to_fit();
		return r;
	}
}
