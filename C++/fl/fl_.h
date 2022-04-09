class br_fstream : public byteReader
{
	std::ifstream fst;
protected:
	uint8_t read1()
	{
		uint8_t r = static_cast<uint8_t>(fst.get());
		pos++;
		return r;
	}
	void readAll(uint8_t* v, const size_t n)
	{
		if(n == 0)
			return;
		fst.read(reinterpret_cast<char*>(v), n);
		pos += n;
	}
public:
	br_fstream() : byteReader(0) {}
	bool open(const char* fl)
	{
		fst.open(fl, std::ios_base::binary | std::ios_base::ate);
		if(fst.fail())
			return false;
		pos = 0;
		size = static_cast<size_t>(fst.tellg());
		fst.seekg(0, std::ios_base::beg);
		return true;
	}

	void set_pos(size_t p)
	{
		fst.seekg(p, std::ios_base::beg);
		pos = p;
	}
	bool skip(size_t n)
	{
		if (pos + n > size)
			return false;
		fst.seekg(n, std::ios_base::cur);
		pos += n;
		return true;
	}
};

class bw_fstream : public byteWriter
{
	std::ofstream fst;
public:
	template<typename C>
	bw_fstream(const C* fl) : fst(fl, std::ios_base::binary) {}

	void writeN(const uint8_t* v, size_t n)
	{
		fst.write(reinterpret_cast<const char*>(v), n);
	}

	void Fin()
	{
		fst.flush();
	}
};
