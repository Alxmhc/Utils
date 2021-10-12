class byteWriter
{
public:
	virtual void writeN(const uint8_t*, size_t) = 0;
	virtual void writeN(const char*, size_t) = 0;

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
	void writeN(const char* v, size_t n)
	{
		s.write(v, n);
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
	void writeN(const char* v, size_t n)
	{
		d.insert(d.end(), v, v + n);
	}
};
