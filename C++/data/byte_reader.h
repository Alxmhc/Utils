class byteReader
{
	bool check(size_t n)
	{
		return get_pos() + n <= sz;
	}
protected:
	size_t sz;
	virtual void readAll(uint8_t*, const size_t) = 0;
public:
	virtual size_t get_pos() const = 0;
	virtual void set_pos(int_fast64_t, std::ios_base::seekdir) = 0;

	virtual bool get(uint8_t&) = 0;
	virtual std::string read_string(char) = 0;

	bool readN(uint8_t* d, size_t n)
	{
		if(n == 0)
			return true;
		if(!check(n))
			return false;
		readAll(d, n);
		return true;
	}
	bool readN_v(std::vector<uint8_t> &v, size_t n)
	{
		if(n == 0)
			return true;
		if(!check(n))
			return false;
		const auto sz = v.size();
		v.resize(sz + n);
		readAll(v.data() + sz, n);
		return true;
	}
};

class br_stream : public byteReader
{
	std::istream &s;
protected:
	void Init()
	{
		s.seekg(0, std::ios_base::end);
		sz = static_cast<size_t>(s.tellg());
		s.seekg(0, std::ios_base::beg);
	}

	void readAll(uint8_t *v, const size_t n)
	{
		s.read(reinterpret_cast<char*>(v), n);
	}
public:
	br_stream(std::istream &d, size_t size) : s(d)
	{
		sz = size;
	}
	br_stream(std::istream &d) : s(d)
	{
		Init();
	}
	size_t get_pos() const
	{
		return static_cast<size_t>(s.tellg());
	}
	void set_pos(int_fast64_t pos, std::ios_base::seekdir t)
	{
		s.seekg(pos, t);
	}

	bool get(uint8_t &b)
	{
		b = static_cast<uint8_t>(s.get());
		return !s.fail();
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
	br_fstream(const char *fl) : br_stream(fst, 0), fst(fl, std::ios_base::binary)
	{
		Init();
	}
};

class br_array : public byteReader
{
	const uint8_t *d;
	size_t o;
protected:
	void readAll(uint8_t *v, const size_t n)
	{
		std::copy_n(d + o, n, v);
		o += n;
	}
public:
	br_array(const uint8_t *v, size_t size) : d(v), o(0)
	{
		sz = size;
	}

	size_t get_pos() const
	{
		return o;
	}
	void set_pos(int_fast64_t pos, std::ios_base::seekdir t)
	{
		switch(t)
		{
		case std::ios_base::beg:
			o = pos;
			break;
		case std::ios_base::cur:
			o += pos;
			break;
		case std::ios_base::end:
			o = sz + pos;
			break;
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

	std::string read_string(char e)
	{
		const auto n = std::find(d + o, d + sz, e) - d;
		std::string r(d + o, d + n);
		o = n + 1;
		return r;
	}
};
