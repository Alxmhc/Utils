class byteReader
{
protected:
	size_t size;
	size_t pos;
	byteReader(size_t sz) : size(sz), pos(0) {}

	virtual uint8_t read1() = 0;
	virtual void readAll(uint8_t*, const size_t) = 0;
	void readAll(char *s, const size_t n)
	{
		readAll(reinterpret_cast<uint8_t*>(s), n);
	}
public:
	size_t get_size() const
	{
		return size;
	}
	size_t get_pos() const
	{
		return pos;
	}
	virtual void set_pos(size_t) = 0;
	virtual bool skip(size_t) = 0;

	bool get(uint8_t &c)
	{
		if(pos >= size)
			return false;
		c = read1();
		return true;
	}

	bool readN(uint8_t* d, size_t n)
	{
		if(pos + n > size)
			return false;
		readAll(d, n);
		return true;
	}
	bool readN(std::vector<uint8_t> &v, size_t n)
	{
		if(pos + n > size)
			return false;
		v.resize(n);
		readAll(v.data(), n);
		return true;
	}
	bool readN(std::string &s, size_t n)
	{
		if(pos + n > size)
			return false;
		s.resize(n);
		readAll(const_cast<char*>(s.data()), n);
		return true;
	}

	size_t readMx(uint8_t* d, size_t n)
	{
		if(pos + n > size)
		{
			n = size - pos;
		}
		readAll(d, n);
		return n;
	}

	bool addN(std::vector<uint8_t> &v, size_t n)
	{
		if(pos + n > size)
			return false;
		if(n != 0)
		{
			const auto sz = v.size();
			v.resize(sz + n);
			readAll(v.data() + sz, n);
		}
		return true;
	}

	template<char E>
	bool readC_2(uint_fast16_t &c)
	{
		uint8_t t[2];
		if (!readN(t, 2))
			return false;
		c = bconv<2, E>::pack(t);
		return true;
	}
	template<char E>
	bool readC_4(uint_fast32_t &c)
	{
		uint8_t t[4];
		if (!readN(t, 4))
			return false;
		c = bconv<4, E>::pack(t);
		return true;
	}
	template<char E>
	bool readC_8(uint_fast64_t &c)
	{
		uint8_t t[8];
		if (!readN(t, 8))
			return false;
		c = bconv<8, E>::pack(t);
		return true;
	}

	bool find1(uint8_t c, size_t &p)
	{
		const auto b = pos;
		while(pos < size)
		{
			if(read1() == c)
			{
				p = pos - 1;
				set_pos(b);
				return true;
			}
		}
		set_pos(b);
		return false;
	}

	bool read_string(uint8_t c, std::string &s)
	{
		s.clear();
		const auto p = pos;
		while(pos < size)
		{
			const auto t = read1();
			if(t == c)
				return true;
			s.push_back(t);
		}
		s.clear();
		set_pos(p);
		return false;
	}
};

class br_stream : public byteReader
{
	std::istream &s;
protected:
	void Init()
	{
		if(s.fail())
			return;
		s.seekg(0, std::ios_base::end);
		size = static_cast<size_t>(s.tellg());
		s.seekg(0, std::ios_base::beg);
	}

	uint8_t read1()
	{
		uint8_t r = static_cast<uint8_t>(s.get());
		pos++;
		return r;
	}
	void readAll(uint8_t *v, const size_t n)
	{
		if(n == 0)
			return;
		s.read(reinterpret_cast<char*>(v), n);
		pos += n;
	}
public:
	br_stream(std::istream &d, size_t sz) : byteReader(sz), s(d) {}
	br_stream(std::istream &d) : byteReader(0), s(d)
	{
		Init();
	}

	void set_pos(size_t p)
	{
		s.seekg(p, std::ios_base::beg);
		pos = p;
	}
	bool skip(size_t n)
	{
		if (pos + n > size)
			return false;
		s.seekg(n, std::ios_base::cur);
		pos += n;
		return true;
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
protected:
	uint8_t read1()
	{
		uint8_t r = d[pos];
		pos++;
		return r;
	}
	void readAll(uint8_t *v, const size_t n)
	{
		if(n == 0)
			return;
		std::copy_n(d + pos, n, v);
		pos += n;
	}
public:
	br_array(const uint8_t *v, size_t sz) : byteReader(sz), d(v) {}

	void set_pos(size_t p)
	{
		pos = p;
	}
	bool skip(size_t n)
	{
		if (pos + n > size)
			return false;
		pos += n;
		return true;
	}
};
