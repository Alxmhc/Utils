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
	byteReader() : pos(0) {}

	virtual uint8_t read1() = 0;
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

	std::size_t readMx(uint8_t* d, std::size_t n)
	{
		if(pos + n > csize)
		{
			n = csize - pos;
		}
		readAll(d, n);
		return n;
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

	template<unsigned char SZ, char E>
	bool readC(typename UINT_<SZ>::uint &c)
	{
		uint8_t t[SZ];
		if (!readN(t, SZ))
			return false;
		c = bconv<1, SZ, E>::pack(t);
		return true;
	}

	std::size_t find(uint8_t c)
	{
		std::size_t res = csize;

		const auto b = pos;
		while(pos < csize)
		{
			if(read1() == c)
			{
				res = pos - 1 - b;
				break;
			}
		}
		set_pos(b);
		return res;
	}

	std::size_t find(const uint8_t* v, std::size_t k)
	{
		std::size_t res = csize;
		if(k > csize)
			return res;

		const auto b = pos;
		for(std::size_t i = pos; i <= csize - k; i++)
		{
			std::size_t j = 0;
			for(; j < k; j++)
			{
				if(read1() != v[j])
					break;
			}
			if(j == 0)
				continue;
			if(j == k)
			{
				res = pos - k - b;
				break;
			}
			set_pos(i+1);
		}
		set_pos(b);
		return res;
	}

	bool read_string(uint8_t c, std::string &s)
	{
		const std::size_t p = find(c);
		if(p == csize)
			return false;
		readN(s, p);
		skip(1);
		return true;
	}

	bool read_string(const uint8_t* v, std::size_t k, std::string &s)
	{
		const std::size_t p = find(v, k);
		if(p == csize)
			return false;
		readN(s, p);
		skip(k);
		return true;
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
	void readAll(uint8_t* v, const std::size_t n)
	{
		if(n == 0)
			return;
		std::copy_n(d + pos, n, v);
		pos += n;
	}
public:
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
};

#endif
