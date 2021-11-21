template<typename T>
bool find1(const T* v, const size_t sz, const T e, size_t &p)
{
	for(p = 0; p < sz; p++)
	{
		if(v[p] == e)
			return true;
	}
	return false;
}

namespace decode
{
	bool unchunk(const uint8_t *v, const size_t n, byteWriter &bw)
	{
		size_t i = 0;
		const uint8_t f('\n');
		for(;;)
		{
			size_t p;
			if( !find1(v + i, n - i, f, p) )
				break;

			auto sz = strtoul(reinterpret_cast<const char*>(v + i), nullptr, 16);
			if(sz == 0)
				return true;

			i += p + 1;
			if(i + sz > n)
				break;
			bw.writeN(v + i, sz);
			i += sz;
			if( !find1(v + i, n - i, f, p) || p > 2 )
				break;
			i += p + 1;
		}
		return false;
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
