#ifndef H_BYTE_WRITER
#define H_BYTE_WRITER

#include <vector>
#include <string>
#include <algorithm>
#include "./pack.h"

class byteWriter
{
public:
	virtual void writeN(const uint8_t*, std::size_t) = 0;

	void write(uint8_t c)
	{
		writeN(&c, 1);
	}
	void writeS(const std::string &s)
	{
		writeN(reinterpret_cast<const uint8_t*>(s.c_str()), s.length());
	}

	template<unsigned char SZ, char E>
	void writeC(typename UINT_<SZ>::uint_ c)
	{
		uint8_t t[SZ];
		bconv<SZ, E>::unpack(c, t);
		writeN(t, SZ);
	}
	template<unsigned char SZ, char E>
	void writeC(typename UINT_<SZ>::uint_ const *c, std::size_t n)
	{
		uint8_t t[SZ];
		for(std::size_t i = 0; i < n; i++)
		{
			bconv<SZ, E>::unpack(c[i], t);
			writeN(t, SZ);
		}
	}

	virtual void Fin()
	{
	}
};

class bw_array : public byteWriter
{
	std::vector<uint8_t> &d;
	void operator=(const bw_array&);
public:
	bw_array(std::vector<uint8_t> &v) : d(v) {}

	void writeN(const uint8_t* v, std::size_t n)
	{
		d.insert(d.end(), v, v + n);
	}
};

template<std::size_t SZ>
class byteWriterBuf : public byteWriter
{
	std::size_t offset;
	uint8_t buf[SZ];
protected:
	virtual void process(const uint8_t*) = 0;

	static const std::size_t bsize = SZ;

	byteWriterBuf() : offset(0) {}

	std::size_t size() const
	{
		return offset;
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
	}

	void pad_PKCS()
	{
		std::fill(buf + offset, buf + SZ, SZ - offset);
		process(buf);
		offset = 0;
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
				offset += n;
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
		offset = n - part;
		std::copy_n(v + part, offset, buf);
	}
};

#endif
