class byteWriter
{
public:
	virtual void write(const uint8_t*, size_t) = 0;
};

class bw_stream : public byteWriter
{
	std::ostream &s;
public:
	bw_stream(std::ostream &d) : s(d) {}

	void write(const uint8_t* v, size_t n)
	{
		s.write(reinterpret_cast<const char*>(v), n);
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

	void write(const uint8_t* v, size_t n)
	{
		d.insert(d.end(), v, v + n);
	}
};
