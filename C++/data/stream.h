namespace stream
{
	std::string read_string(std::istream &s)
	{
		std::string r;
		for(;;)
		{
			char c = s.get();
			if(c == 0)
				break;
			r += c;
		}
		return r;
	}

	template<char E, typename T>
	void read(std::istream &s, T &c)
	{
		char t[sizeof(T)];
		s.read(t, sizeof(T));
		conv::read<E>(reinterpret_cast<const uint8_t*>(t), c);
	}

	void append(std::istream &d, std::vector<uint8_t> &v, std::size_t n)
	{
		auto end = v.size();
		v.resize(end + n);
		d.read(reinterpret_cast<char*>(v.data() + end), n);
	}
}
