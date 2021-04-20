class byteReader
{
protected:
	size_t sz;
	virtual size_t read(uint8_t*, size_t) = 0;
public:
	virtual size_t get_size() = 0;
	virtual size_t get_pos() const = 0;
	virtual void set_pos(int_fast64_t, std::ios_base::seekdir) = 0;

	virtual bool get(uint8_t&) = 0;
	virtual std::string read_string(char) = 0;

	bool readN(uint8_t* d, size_t n)
	{
		if(n == 0)
			return true;
		return read(d, n) == n;
	}
	bool readN_v(std::vector<uint8_t> &v, size_t n)
	{
		if(n == 0)
			return true;
		std::vector<uint8_t> t(n);
		if(read(t.data(), n) != n)
			return false;
		v.insert(v.end(), t.begin(), t.end());
		return true;
	}
};

class br_stream : public byteReader
{
	std::istream &s;
protected:
	size_t read(uint8_t *v, size_t n)
	{
		s.read(reinterpret_cast<char*>(v), n);
		return static_cast<size_t>(s.gcount());
	}
public:
	br_stream(std::istream &d, size_t size = 0) : s(d)
	{
		sz = size;
	}

	size_t get_size()
	{
		if(sz == 0)
		{
			auto pos = s.tellg();
			s.seekg(0, std::ios_base::end);
			sz = static_cast<size_t>(s.tellg());
			s.seekg(pos, std::ios_base::beg);
		}
		return sz;
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
	br_fstream(const char *fl) : br_stream(fst), fst(fl, std::ios_base::binary) {}
};

class br_array : public byteReader
{
	const uint8_t *d;
	size_t o;
protected:
	size_t read(uint8_t *v, size_t n)
	{
		n = std::min(n, sz - o);
		std::copy_n(d + o, n, v);
		o += n;
		return n;
	}
public:
	br_array(const uint8_t *v, size_t size) : d(v), o(0)
	{
		sz = size;
	}

	size_t get_size()
	{
		return sz;
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
