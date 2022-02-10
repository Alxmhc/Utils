class byteWriter
{
public:
	virtual void writeN(const uint8_t*, size_t) = 0;
	void writeN(const char* s, size_t n)
	{
		writeN(reinterpret_cast<const uint8_t*>(s), n);
	}

	void write(uint8_t c)
	{
		writeN(&c, 1);
	}
	void write(const std::string &s)
	{
		writeN(s.c_str(), s.length());
	}

	template<char E>
	void writeC_2(uint_fast16_t c)
	{
		uint8_t t[2];
		bconv<2, E>::unpack(c, t);
		writeN(t, 2);
	}
	template<char E>
	void writeC_4(uint_fast32_t c)
	{
		uint8_t t[4];
		bconv<4, E>::unpack(c, t);
		writeN(t, 4);
	}
	template<char E>
	void writeC_8(uint_fast64_t c)
	{
		uint8_t t[8];
		bconv<8, E>::unpack(c, t);
		writeN(t, 8);
	}

	template<char E>
	void writeC_2(const uint_fast16_t *c, size_t n)
	{
		uint8_t t[2];
		for(size_t i = 0; i < n; i++)
		{
			bconv<2, E>::unpack(c[i], t);
			writeN(t, 2);
		}
	}
	template<char E>
	void writeC_4(const uint_fast32_t *c, size_t n)
	{
		uint8_t t[4];
		for(size_t i = 0; i < n; i++)
		{
			bconv<4, E>::unpack(c[i], t);
			writeN(t, 4);
		}
	}
	template<char E>
	void writeC_8(const uint_fast64_t *c, size_t n)
	{
		uint8_t t[8];
		for(size_t i = 0; i < n; i++)
		{
			bconv<8, E>::unpack(c[i], t);
			writeN(t, 8);
		}
	}

	virtual void Fin()
	{
	}
};

class bw_stream : public byteWriter
{
	std::ostream &s;
public:
	bw_stream(std::ostream &d) : s(d) {}

	void writeN(const uint8_t* v, size_t n)
	{
		s.write(reinterpret_cast<const char*>(v), n);
	}

	void Fin()
	{
		s.flush();
	}
};

class bw_fstream : public bw_stream
{
	std::ofstream fst;
public:
	template<typename C>
	bw_fstream(const C *fl) : bw_stream(fst), fst(fl, std::ios_base::binary) {}
};

class bw_array : public byteWriter
{
	std::vector<uint8_t> &d;
public:
	bw_array(std::vector<uint8_t> &v) : d(v) {}

	void writeN(const uint8_t* v, size_t n)
	{
		d.insert(d.end(), v, v + n);
	}
};

template<size_t SZ>
class byteWriterBuf : public byteWriter
{
protected:
	uint8_t buf[SZ];
	size_t offset;
	virtual void process(const uint8_t*) = 0;
public:
	static const size_t bsize = SZ;

	byteWriterBuf() : offset(0) {}

	void writeN(const uint8_t* v, size_t n)
	{
		if(n + offset < SZ)
		{
			std::copy_n(v, n, buf + offset);
			offset += n;
			return;
		}
		size_t part = 0;
		if(offset != 0)
		{
			part = SZ - offset;
			std::copy_n(v, part, buf + offset);
			process(buf);
		}
		for (; part + SZ <= n; part += SZ)
		{
			process(v + part);
		}
		offset = n - part;
		std::copy_n(v + part, offset, buf);
	}
};
