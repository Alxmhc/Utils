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

	std::vector<uint8_t> read_vector(std::istream &s, std::size_t n)
	{
		std::vector<uint8_t> res(n);
		s.read(reinterpret_cast<char*>(res.data()), n);
		return res;
	}

	template<char E, typename T>
	void read(std::istream &s, T &c)
	{
		char t[sizeof(T)];
		s.read(t, sizeof(T));
		conv::read<E>(reinterpret_cast<const uint8_t*>(t), c);
	}

	void append(std::istream &s, std::vector<uint8_t> &v, std::size_t n)
	{
		auto end = v.size();
		v.resize(end + n);
		s.read(reinterpret_cast<char*>(v.data() + end), n);
	}

	void copy_n(std::istream &in, std::size_t n, std::ostream &out)
	{
		while(n != 0)
		{
			--n;
			out.put( in.get() );
		}
	}
}
