class byteWriter
{
public:
	virtual void write(const uint8_t*, std::size_t) = 0;
};

class bw_  : public byteWriter
{
	std::vector<byteWriter*> m;
public:
	bw_(){}
	void add(byteWriter& w)
	{
		m.push_back(&w);
	}
	void write(const uint8_t* v, std::size_t n)
	{
		for(std::size_t i = 0; i < m.size(); i++)
		{
			m[i]->write(v, n);
		}
	}
};

class bw_stream : public byteWriter
{
	std::ostream &s;
public:
	bw_stream(std::ostream &d) : s(d) {}

	void write(const uint8_t* v, std::size_t n)
	{
		s.write(reinterpret_cast<const char*>(v), n);
	}
};

class bw_fstream : public bw_stream
{
	std::ofstream fst;
public:
	bw_fstream(const char *fl) : bw_stream(fst), fst(fl, std::ios_base::binary) {}
};

template<class H>
class bw_hash : public byteWriter
{
	H hash;
public:
	bw_hash(){}

	void write(const uint8_t* v, std::size_t n)
	{
		hash.Update(v, n);
	}
	bool check(const uint8_t* v)
	{
		uint8_t h[H::hash_size];
		hash.Final(h);
		uint8_t r = 0;
		for(int i = 0; i < H::hash_size; i++)
		{
			r |= (v[i] ^ h[i]);
		}
		return r == 0;
	}
};
