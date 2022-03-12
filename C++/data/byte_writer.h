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

	template<unsigned char SZ, char E>
	void writeC(typename UINT_<SZ>::uint_ c)
	{
		uint8_t t[SZ];
		bconv<SZ, E>::unpack(c, t);
		writeN(t, SZ);
	}
	template<unsigned char SZ, char E>
	void writeC(typename UINT_<SZ>::uint_ const *c, size_t n)
	{
		uint8_t t[SZ];
		for(size_t i = 0; i < n; i++)
		{
			bconv<SZ, E>::unpack(c[i], t);
			writeN(t, SZ);
		}
	}

	virtual void Fin()
	{
	}
};

class bw_fstream : public byteWriter
{
	std::ofstream fst;
public:
	template<typename C>
	bw_fstream(const C *fl) : fst(fl, std::ios_base::binary) {}

	void writeN(const uint8_t* v, size_t n)
	{
		fst.write(reinterpret_cast<const char*>(v), n);
	}

	void Fin()
	{
		fst.flush();
	}
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
	size_t offset;
	uint8_t buf[SZ];
protected:
	virtual void process(const uint8_t*) = 0;

	static const size_t bsize = SZ;

	byteWriterBuf() : offset(0) {}

	size_t size() const
	{
		return offset;
	}

	const uint8_t* data() const
	{
		return buf;
	}

	void reset()
	{
		offset = 0;
	}

	void nul()
	{
		std::fill(buf + offset, buf + SZ, 0);
	}
public:
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
