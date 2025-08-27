#ifndef H_BYTE_READER
#define H_BYTE_READER

#include <vector>
#include <string>
#include <algorithm>

#include "pack.h"

class byteReader
{
protected:
	std::size_t pos;
	std::size_t size, csize;
	byteReader() : pos(0), size(0), csize(0) {}

	virtual uint8_t read1() = 0;
	virtual const uint8_t* get_data(uint_fast8_t) = 0;
	virtual void readAll(uint8_t*, const std::size_t) = 0;
public:
	bool set_size(std::size_t sz)
	{
		if(sz > size)
			return false;
		csize = sz;
		return true;
	}
	bool set_rsize(std::size_t sz)
	{
		return set_size(sz + pos);
	}
	void reset_size()
	{
		csize = size;
	}

	std::size_t get_size() const
	{
		return csize;
	}
	std::size_t get_rsize() const
	{
		return csize - pos;
	}
	std::size_t get_pos() const
	{
		return pos;
	}

	virtual void set_pos(std::size_t) = 0;
	virtual bool skip(std::size_t) = 0;

	bool get(uint8_t &c)
	{
		if(pos >= csize)
			return false;
		c = read1();
		return true;
	}

	bool readN(uint8_t* d, std::size_t n)
	{
		if(pos + n > csize)
			return false;
		readAll(d, n);
		return true;
	}
	bool readN(std::vector<uint8_t> &v, std::size_t n)
	{
		if(pos + n > csize)
			return false;
		v.resize(n);
		readAll(v.data(), n);
		return true;
	}
	bool readN(std::string &s, std::size_t n)
	{
		if(pos + n > csize)
			return false;
		s.resize(n);
		char* t = const_cast<char*>(s.data());
		readAll(reinterpret_cast<uint8_t*>(t), n);
		return true;
	}

	bool addN(std::vector<uint8_t> &v, std::size_t n)
	{
		if(pos + n > csize)
			return false;
		if(n != 0)
		{
			const auto sz = v.size();
			v.resize(sz + n);
			readAll(v.data() + sz, n);
		}
		return true;
	}

	std::size_t readMx(uint8_t* d, std::size_t n)
	{
		if(pos + n > csize)
		{
			n = csize - pos;
		}
		readAll(d, n);
		return n;
	}
	std::size_t addMx(std::vector<uint8_t> &v, std::size_t n)
	{
		if(pos + n > csize)
		{
			n = csize - pos;
		}
		if(n != 0)
		{
			const auto sz = v.size();
			v.resize(sz + n);
			readAll(v.data() + sz, n);
		}
		return n;
	}

	template<unsigned char SZ, char E>
	bool readC(typename UINT_<SZ>::uint &c)
	{
		auto t = get_data(SZ);
		if(t == nullptr)
			return false;
		c = bconv<1, SZ, E>::pack(t);
		return true;
	}

	virtual std::size_t find(uint8_t) = 0;
	virtual std::size_t find(const uint8_t*, uint_fast8_t) = 0;

	bool read_string(const uint8_t* e, uint_fast8_t k, std::string &s)
	{
		s.clear();
		const auto n = find(e, k);
		if(n == get_rsize())
			return false;
		readN(s, n);
		skip(k);
		return true;
	}

	bool read_string(uint8_t e, std::string &s)
	{
		return read_string(&e, 1, s);
	}
};

class br_array : public byteReader
{
	const uint8_t* d;
protected:
	uint8_t read1()
	{
		uint8_t r = d[pos];
		pos++;
		return r;
	}
	const uint8_t* get_data(uint_fast8_t n) override
	{
		if (pos + n > csize)
			return nullptr;
		const uint8_t* res = d + pos;
		pos += n;
		return res;
	}
	void readAll(uint8_t* v, const std::size_t n)
	{
		if(n == 0)
			return;
		std::copy_n(d + pos, n, v);
		pos += n;
	}
public:
	br_array() : d(nullptr) {}

	br_array(const uint8_t* v, std::size_t sz) : d(v)
	{
		size = sz;
		csize = size;
	}

	void open(const uint8_t* v, std::size_t sz)
	{
		d = v;
		pos = 0;
		size = sz;
		csize = size;
	}

	void set_pos(std::size_t p)
	{
		pos = p;
	}
	bool skip(std::size_t n)
	{
		if (pos + n > csize)
			return false;
		pos += n;
		return true;
	}

	std::size_t find(uint8_t e)
	{
		return std::find(d + pos, d + csize, e) - (d + pos);
	}
	std::size_t find(const uint8_t* e, uint_fast8_t k)
	{
		if(k == 1)
			return find(*e);
		return std::search(d + pos, d + csize, e, e + k) - (d + pos);
	}
};

#endif
