#ifndef H_FL_
#define H_FL_

#include <fstream>

#include "../data/byte_reader.h"
#include "../data/byte_writer.h"

class br_fstream : public byteReader
{
	std::ifstream fst;
	uint8_t buf[256];
protected:
	uint8_t read1()
	{
		uint8_t r = static_cast<uint8_t>(fst.get());
		pos++;
		return r;
	}
	const uint8_t* get_data(uint_fast8_t n) override
	{
		if (pos + n > csize)
			return nullptr;
		fst.read(reinterpret_cast<char*>(buf), n);
		pos += n;
		return buf;
	}
	void readAll(uint8_t* v, const std::size_t n)
	{
		if(n == 0)
			return;
		fst.read(reinterpret_cast<char*>(v), n);
		pos += n;
	}
public:
	bool open(const char* fl)
	{
		fst.close();
		fst.open(fl, std::ios_base::binary | std::ios_base::ate);
		if(fst.fail())
			return false;
		pos = 0;
		size = static_cast<std::size_t>(fst.tellg());
		csize = size;
		fst.seekg(0, std::ios_base::beg);
		return true;
	}
	void close()
	{
		fst.close();
	}

	void set_pos(std::size_t p)
	{
		if(p != pos)
		{
			fst.seekg(p, std::ios_base::beg);
			pos = p;
		}
	}
	bool skip(std::size_t n)
	{
		if(n == 0)
			return true;
		if (pos + n > csize)
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
	bw_fstream(const C* fl, bool append = false) : fst(fl, std::ios_base::binary | (append ? std::ios_base::app : 0)) {}

	void writeN(const uint8_t* v, std::size_t n)
	{
		fst.write(reinterpret_cast<const char*>(v), n);
	}

	void Fin()
	{
		fst.flush();
	}
};

namespace fl_pr
{
	template<class C>
	class flr : public C
	{
		br_fstream f;
	public:
		bool open(const char* fl)
		{
			if( !f.open(fl) )
				return false;
			if( !this->read(&f) )
				return false;
			return true;
		}
	};
}

#endif
