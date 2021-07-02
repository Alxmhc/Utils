class bitReader
{
protected:
	byteReader *r;
	uint_fast8_t o, b;

	bitReader(byteReader &d) : r(&d), o(0) {}
public:
	virtual uint_fast8_t read1() = 0;

	bool readB(uint8_t *v, size_t n)
	{
		o = 0;
		return r->readN(v, n);
	}
	bool addB(std::vector<uint8_t> &v, size_t n)
	{
		o = 0;
		return r->addN(v, n);
	}
	void skipB(size_t n)
	{
		o = 0;
		r->set_pos(n, std::ios_base::cur);
	}
};

//01234567
class bitReaderR : public bitReader
{
public:
	uint_fast8_t read1()
	{
		if(o != 0)
		{
			uint_fast8_t rs = b >> (7 - o);
			o = (o + 1) & 7;
			return rs & 1;
		}
		r->get(b);
		o = 1;
		return b >> 7;
	}

	bitReaderR(byteReader &d) : bitReader(d){}

	uint_fast32_t readLE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			auto c = read1();
			rs |= c << i;
		}
		return rs;
	}

	uint_fast32_t readBE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			auto c = read1();
			rs = (rs<<1) | c;
		}
		return rs;
	}
};

//76543210
class bitReaderL : public bitReader
{
public:
	uint_fast8_t read1()
	{
		if(o != 0)
		{
			uint_fast8_t rs = b >> o;
			o = (o + 1) & 7;
			return rs & 1;
		}
		r->get(b);
		o = 1;
		return b & 1;
	}

	bitReaderL(byteReader &d) : bitReader(d){}

	uint_fast32_t readLE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			auto c = read1();
			rs = (rs<<1) | c;
		}
		return rs;
	}

	uint_fast32_t readBE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			auto c = read1();
			rs |= c << i;
		}
		return rs;
	}
};
