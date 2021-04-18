class byteReader
{
public:
	virtual size_t get_pos() const = 0;
	virtual void set_pos(int_fast32_t) = 0;

	virtual size_t read(uint8_t*, size_t) = 0;
	bool readN(uint8_t* d, size_t n)
	{
		if(n == 0)
			return true;
		return read(d, n) == n;
	}

	size_t read_v(std::vector<uint8_t> &v, size_t n)
	{
		if(n == 0)
			return 0;
		auto sz = v.size();
		v.resize(sz + n);
		return read(&v[sz], n);
	}

	virtual bool get(uint8_t&) = 0;
	virtual void skip(size_t) = 0;
	virtual std::string read_string(char) = 0;

	template<char E, typename T>
	bool getC(T &c)
	{
		const auto sz = sizeof(T);
		uint8_t t[sz];
		const size_t k = read(t, sz);
		if(k != sz)
			return false;
		c = bconv<sz, E>::pack(t);
		return true;
	}
};

class br_stream : public byteReader
{
	std::istream &s;
public:
	br_stream(std::istream &d) : s(d) {}

	size_t get_pos() const
	{
		return static_cast<size_t>(s.tellg());
	}
	void set_pos(int_fast32_t p)
	{
		if(p < 0)
		{
			s.seekg(p, std::ios_base::end);
		}
		else
		{
			s.seekg(p, std::ios_base::beg);
		}
	}

	bool get(uint8_t &b)
	{
		b = s.get();
		return !s.fail();
	}

	void skip(size_t n)
	{
		s.seekg(n, std::ios_base::cur);
	}

	size_t read(uint8_t *v, size_t n)
	{
		s.read(reinterpret_cast<char*>(v), n);
		return static_cast<size_t>(s.gcount());
	}

	std::string read_string(char e)
	{
		std::string r;
		std::getline(s, r, e);
		return r;
	}
};

class br_fstream : public br_stream
{
	std::ifstream fst;
public:
	br_fstream(const char *fl) : br_stream(fst), fst(fl, std::ios_base::binary) {}
};

class br_array : public byteReader
{
	const uint8_t *d;
	size_t sz, o;
public:
	br_array(const uint8_t *v, size_t size) : d(v), sz(size), o(0) {}

	size_t get_pos() const
	{
		return o;
	}
	void set_pos(int_fast32_t p)
	{
		if(p < 0)
		{
			o = sz + p;
		}
		else
		{
			o = p;
		}
	}

	bool get(uint8_t &b)
	{
		if(o >= sz)
			return false;
		b = d[o];
		o++;
		return true;
	}

	void skip(size_t n)
	{
		o += n;
	}

	size_t read(uint8_t *v, size_t n)
	{
		n = std::min(n, sz - o);
		std::copy_n(d + o, n, v);
		o += n;
		return n;
	}

	std::string read_string(char e)
	{
		const auto n = std::find(d + o, d + sz, e) - d;
		std::string r(d + o, d + n);
		o = n + 1;
		return r;
	}
};
