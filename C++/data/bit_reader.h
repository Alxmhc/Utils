class bitReader
{
protected:
	byteReader *r;
	uint_fast8_t o, b;

	bitReader(byteReader &d) : r(&d), o(0) {}
public:
	virtual bool read1(uint_fast8_t&) = 0;

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
	bool skipB(size_t n)
	{
		o = 0;
		return r->skip(n);
	}

	template<unsigned char SZ, char E>
	bool readB_C(typename UINT_<SZ>::uint_ &c)
	{
		o = 0;
		return r->readC<SZ, E>(c);
	}
};

//01234567
class bitReaderR : public bitReader
{
public:
	bool read1(uint_fast8_t &c)
	{
		if(o != 0)
		{
			c = (b >> (7 - o)) & 1;
			o = (o + 1) & 7;
			return true;
		}
		if( !r->get(b) )
			return false;
		c = b >> 7;
		o = 1;
		return true;
	}

	bitReaderR(byteReader &d) : bitReader(d){}

	uint_fast32_t readLE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			read1(c);
			rs |= c << i;
		}
		return rs;
	}

	uint_fast32_t readBE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			read1(c);
			rs = (rs<<1) | c;
		}
		return rs;
	}
};

//76543210
class bitReaderL : public bitReader
{
public:
	bool read1(uint_fast8_t &c)
	{
		if(o != 0)
		{
			c = (b >> o) & 1;
			o = (o + 1) & 7;
			return true;
		}
		if( !r->get(b) )
			return false;
		c = b & 1;
		o = 1;
		return true;
	}

	bitReaderL(byteReader &d) : bitReader(d){}

	uint_fast32_t readLE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			read1(c);
			rs = (rs<<1) | c;
		}
		return rs;
	}

	uint_fast32_t readBE(uint_fast8_t n)
	{
		uint_fast32_t rs = 0;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			read1(c);
			rs |= c << i;
		}
		return rs;
	}
};
