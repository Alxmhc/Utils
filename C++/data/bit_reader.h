class bitReader
{
protected:
	byteReader *r;
	uint_fast8_t o, b;

	bitReader(byteReader &d) : r(&d), o(0) {}
public:
	virtual uint_fast8_t read1() = 0;

	template<char E, typename T>
	void getB(T &c)
	{
		o = 0;
		r->getC<E>(c);
	}

	void readB(uint8_t *v, std::size_t n)
	{
		o = 0;
		r->read(v, n);
	}

	void skipB(std::size_t n)
	{
		o = 0;
		r->skip(n);
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
			uint_fast8_t r = b >> (7 - o);
			o = (o + 1) & 7;
			return r & 1;
		}
		r->get(b);
		o = 1;
		return b >> 7;
	}

	bitReaderR(byteReader &d) : bitReader(d){}

	uint_fast32_t readLE(uint_fast8_t n)
	{
		uint_fast32_t r = 0;
		for(uint_fast8_t i=0; i<n; ++i)
		{
			auto c = read1();
			r |= c << i;
		}
		return r;
	}

	uint_fast32_t readBE(uint_fast8_t n)
	{
		uint_fast32_t r = 0;
		for(uint_fast8_t i=0; i<n; ++i)
		{
			auto c = read1();
			r = (r<<1) | c;
		}
		return r;
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
			uint_fast8_t r = b >> o;
			o = (o + 1) & 7;
			return r & 1;
		}
		r->get(b);
		o = 1;
		return b & 1;
	}

	bitReaderL(byteReader &d) : bitReader(d){}

	uint_fast32_t readLE(uint_fast8_t n)
	{
		uint_fast32_t r = 0;
		for(uint_fast8_t i=0; i<n; ++i)
		{
			auto c = read1();
			r = (r<<1) | c;
		}
		return r;
	}

	uint_fast32_t readBE(uint_fast8_t n)
	{
		uint_fast32_t r = 0;
		for(uint_fast8_t i=0; i<n; ++i)
		{
			auto c = read1();
			r |= c << i;
		}
		return r;
	}
};
