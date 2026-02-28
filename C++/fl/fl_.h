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
	void pos_set(std::size_t p)
	{
		fst.seekg(p);
		pos = p;
	}

	uint8_t read1()
	{
		uint8_t r = static_cast<uint8_t>(fst.get());
		pos++;
		return r;
	}
	const uint8_t* get_data(uint_fast8_t n)
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
		fst.seekg(0);
		return true;
	}
	void close()
	{
		fst.close();
	}

	std::size_t find(uint8_t e)
	{
		auto r = get_rsize();
		const auto p = pos;
		while(pos < csize)
		{
			const auto c = read1();
			if(c == e)
			{
				r = pos - 1 - p;
				break;
			}
		}
		set_pos(p);
		return r;
	}
	std::size_t find(const uint8_t* e, uint_fast8_t k)
	{
		if(k == 1)
			return find(*e);
		auto r = get_rsize();
		const auto p = pos;
		k--;
		while(pos + k < csize)
		{
			const auto c = read1();
			if(c != *e)
				continue;
			if(std::memcmp(get_data(k), e+1, k) == 0)
			{
				r = pos - 1 - k - p;
				break;
			}
			set_pos(pos - k);
		}
		set_pos(p);
		return r;
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
		void close()
		{
			f.close();
		}
	};
}

#endif
