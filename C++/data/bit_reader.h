class bitReader
{
protected:
	std::istream &s;
	uint_fast8_t o, b;

	bitReader(std::istream &d) : o(0), s(d)	{}
public:
	virtual uint_fast8_t read1() = 0;

	template<char E, typename T>
	void getB(T &c)
	{
		o = 0;
		stream::read<E>(s, c);
	}
	void copyB(std::size_t n, std::ostream &out)
	{
		o = 0;
		stream::copy_n(s, n, out);
	}
	void skipB(std::size_t n)
	{
		o = 0;
		s.seekg(n, std::ios_base::cur);
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
		b = s.get();
		o = 1;
		return b >> 7;
	}

	bitReaderR(std::istream &d) : bitReader(d){}

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
		b = s.get();
		o = 1;
		return b & 1;
	}

	bitReaderL(std::istream &d) : bitReader(d){}

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
