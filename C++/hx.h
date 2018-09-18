namespace hex
{
	template <class T>
	std::string to_str(T x)
	{
		const char str[] = "0123456789abcdef";
		auto sz = sizeof(T)*2;
		std::string r(sz, '0');
		while(x)
		{
			r[--sz] = str[x & 0x0f];
			x >>= 4;
		}
		return r;
	}

	template<typename T>
	std::vector<T> from_str(const std::string &str)
	{
		auto sz = str.size();
		auto ch = sizeof(T)*2;
		std::vector<T> n;
		while(sz > ch)
		{
			sz -= ch;
			auto st = str.substr(sz, ch);
			n.push_back(static_cast<T>(strtoul(st.c_str(), nullptr, 16)));
		}
		auto st = str.substr(0, sz);
		n.push_back(static_cast<T>(strtoul(st.c_str(), nullptr, 16)));
		return n;
	}
}
