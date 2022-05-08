class bitReader
{
protected:
	byteReader* r;
	uint_fast8_t o, b;

	bitReader(byteReader &d) : r(&d), o(0) {}
public:
	virtual bool get(uint_fast8_t&) = 0;

	bool readB(uint8_t* v, size_t n)
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
	bool get(uint_fast8_t &c)
	{
		if(o == 0)
		{
			if( !r->get(b) )
				return false;
			b = bitRevT[b];
		}
		c = b & 1;
		b >>= 1;
		o = (o + 1) & 7;
		return true;
	}

	bitReaderR(byteReader &d) : bitReader(d){}

	template<typename T>
	bool readLE(uint_fast8_t n, T &rs)
	{
		rs = 0;
		if(n == 0)
			return true;
		if(n > (sizeof(T)<<3))
			return false;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			if( !get(c) )
				return false;
			rs |= static_cast<T>(c) << i;
		}
		return true;
	}

	template<typename T>
	bool readBE(uint_fast8_t n, T &rs)
	{
		rs = 0;
		if(n == 0)
			return true;
		if(n > (sizeof(T)<<3))
			return false;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			if( !get(c) )
				return false;
			rs = (rs<<1) | c;
		}
		return true;
	}
};

//76543210
class bitReaderL : public bitReader
{
public:
	bool get(uint_fast8_t &c)
	{
		if(o == 0)
		{
			if( !r->get(b) )
				return false;
		}
		c = b & 1;
		b >>= 1;
		o = (o + 1) & 7;
		return true;
	}

	bitReaderL(byteReader &d) : bitReader(d){}

	template<typename T>
	bool readLE(uint_fast8_t n, T &rs)
	{
		rs = 0;
		if(n == 0)
			return true;
		if(n > (sizeof(T)<<3))
			return false;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			if( !get(c) )
				return false;
			rs = (rs<<1) | c;
		}
		return true;
	}

	template<typename T>
	bool readBE(uint_fast8_t n, T &rs)
	{
		rs = 0;
		if(n == 0)
			return true;
		if(n > (sizeof(T)<<3))
			return false;
		for(uint_fast8_t i = 0; i < n; ++i)
		{
			uint_fast8_t c;
			if( !get(c) )
				return false;
			rs |= static_cast<T>(c) << i;
		}
		return true;
	}
};
