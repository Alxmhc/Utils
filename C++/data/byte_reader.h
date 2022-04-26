class byteReader
{
protected:
	size_t size;
	size_t pos;
	byteReader(size_t sz) : size(sz), pos(0) {}

	virtual uint8_t read1() = 0;
	virtual void readAll(uint8_t*, const size_t) = 0;
public:
	size_t get_size() const
	{
		return size;
	}
	size_t get_pos() const
	{
		return pos;
	}
	size_t get_rsize() const
	{
		return size - pos;
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
		char* t = const_cast<char*>(s.data());
		readAll(reinterpret_cast<uint8_t*>(t), n);
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

	template<unsigned char SZ, char E>
	bool readC(typename UINT_<SZ>::uint_ &c)
	{
		uint8_t t[SZ];
		if (!readN(t, SZ))
			return false;
		c = bconv<SZ, E>::pack(t);
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

class br_array : public byteReader
{
	const uint8_t* d;
protected:
	uint8_t read1()
	{
		uint8_t r = d[pos];
		pos++;
		return r;
	}
	void readAll(uint8_t* v, const size_t n)
	{
		if(n == 0)
			return;
		std::copy_n(d + pos, n, v);
		pos += n;
	}
public:
	br_array(const uint8_t* v, size_t sz) : byteReader(sz), d(v) {}

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
