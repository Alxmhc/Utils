class byteReader
{
protected:
	size_t size;
	size_t pos;
	byteReader(size_t sz) : size(sz), pos(0) {}

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
	virtual void set_pos(size_t) = 0;
	virtual void skip(size_t) = 0;

	virtual bool get(uint8_t&) = 0;
	virtual std::string read_string(char) = 0;

	bool readN(uint8_t* d, size_t n)
	{
		if(pos + n > size)
			return false;
		if(n != 0)
		{
			readAll(d, n);
		}
		return true;
	}
	bool readN(std::vector<uint8_t> &v, size_t n)
	{
		if(pos + n > size)
			return false;
		v.resize(n);
		if(n != 0)
		{
			readAll(v.data(), n);
		}
		return true;
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

	void readAll(uint8_t *v, const size_t n)
	{
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
	void skip(size_t p)
	{
		s.seekg(p, std::ios_base::cur);
		pos += p;
	}

	bool get(uint8_t &b)
	{
		if(pos >= size)
			return false;
		b = static_cast<uint8_t>(s.get());
		pos++;
		return true;
	}

	std::string read_string(char e)
	{
		std::string r;
		std::getline(s, r, e);
		pos += r.length() + 1;
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
protected:
	void readAll(uint8_t *v, const size_t n)
	{
		std::copy_n(d + pos, n, v);
		pos += n;
	}
public:
	br_array(const uint8_t *v, size_t sz) : byteReader(sz), d(v) {}

	void set_pos(size_t p)
	{
		pos = p;
	}
	void skip(size_t p)
	{
		pos += p;
	}

	bool get(uint8_t &b)
	{
		if(pos >= size)
			return false;
		b = d[pos];
		pos++;
		return true;
	}

	std::string read_string(char e)
	{
		const auto n = std::find(d + pos, d + size, e) - d;
		std::string r(d + pos, d + n);
		pos = n + 1;
		return r;
	}
};
