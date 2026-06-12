#ifndef H_BYTE_WRITER
#define H_BYTE_WRITER

#include <vector>
#include <string>

#include "pack.h"

class byteWriter
{
public:
	virtual void writeN(const uint8_t*, std::size_t) = 0;

	virtual void write(uint8_t c)
	{
		writeN(&c, 1);
	}

	void writeS(const char* s, std::size_t n)
	{
		writeN(reinterpret_cast<const uint8_t*>(s), n);
	}
	void writeS(const std::string &s)
	{
		writeS(s.c_str(), s.length());
	}

	template<unsigned char SZ, char E>
	void writeC(typename UINT_<SZ>::uint c)
	{
		uint8_t t[SZ];
		bconv<1, E>::unpack(c, SZ, t);
		writeN(t, SZ);
	}

	virtual void Fin()
	{
	}
};

class bw_vector : public byteWriter
{
	std::vector<uint8_t> &d;
	bw_vector& operator=(const bw_vector&);
public:
	bw_vector(std::vector<uint8_t> &v) : d(v) {}

	void write(uint8_t c)
	{
		d.push_back(c);
	}

	void writeN(const uint8_t* v, std::size_t n)
	{
		d.insert(d.end(), v, v + n);
	}
};
class bw_string : public byteWriter
{
	std::string &d;
	bw_string& operator=(const bw_string&);
public:
	bw_string(std::string &s) : d(s) {}

	void write(uint8_t c)
	{
		d.push_back(c);
	}

	void writeN(const uint8_t* v, std::size_t n)
	{
		d.insert(d.end(), v, v + n);
	}
};

template<uint8_t SZ>
class byteWriterBuf : public byteWriter
{
	uint8_t offset;
	uint8_t buf[SZ];
protected:
	virtual void process(const uint8_t*) = 0;

	static const uint8_t bsize = SZ;

	byteWriterBuf() : offset(0) {}

	uint8_t size() const
	{
		return offset;
	}
	uint8_t rsize() const
	{
		return SZ - offset;
	}

	const uint8_t* data() const
	{
		return buf;
	}

	void reset()
	{
		offset = 0;
	}

	void fill_e(uint8_t c)
	{
		std::fill(buf + offset, buf + SZ, c);
		offset = 0;
	}

	void pad_Const(uint8_t c)
	{
		std::fill(buf + offset, buf + SZ, c);
		process(buf);
		offset = 0;
	}
	void pad_PKCS()
	{
		pad_Const(SZ - offset);
	}
public:
	void write(uint8_t c)
	{
		buf[offset] = c;
		offset++;
		if(offset == SZ)
		{
			process(buf);
			offset = 0;
		}
	}

	void writeN(const uint8_t* v, std::size_t n)
	{
		if(n + offset < SZ)
		{
			if(n != 0)
			{
				std::copy_n(v, n, buf + offset);
				offset += static_cast<uint8_t>(n);
			}
			return;
		}
		std::size_t part = 0;
		if(offset != 0)
		{
			part = SZ - offset;
			std::copy_n(v, part, buf + offset);
			process(buf);
		}
		for (; part + SZ <= n; part += SZ)
		{
			process(v + part);
		}
		offset = static_cast<uint8_t>(n - part);
		std::copy_n(v + part, offset, buf);
	}
};

#endif
