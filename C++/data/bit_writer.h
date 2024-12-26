#ifndef H_BIT_WRITER
#define H_BIT_WRITER

#include "byte_writer.h"

class bitWriter
{
protected:
	byteWriter* r;
	uint_fast8_t o, b;

	bitWriter(byteWriter &d) : r(&d), o(0), b(0) {}

	void wre()
	{
		r->write(b);
		b = 0;
		o = 0;
	}
public:
	void pad0()
	{
		if(o != 0)
		{
			wre();
		}
	}

	void writeN(const uint8_t* v, std::size_t n)
	{
		r->writeN(v, n);
	}

	template<unsigned char SZ, char E>
	void writeC(typename UINT_<SZ>::uint c)
	{
		uint8_t t[SZ];
		bconv<1, SZ, E>::unpack(c, t);
		r->writeN(t, SZ);
	}
};

//01234567
class bitWriterR : public bitWriter
{
public:
	bitWriterR(byteWriter &d) : bitWriter(d){}

	template<typename T>
	void writeLE(T c, uint_fast8_t n)
	{
		while(n != 0)
		{
			n--;
			b |= (c & 1) << (7 - o);
			c >>= 1;
			o++;
			if(o == 8)
			{
				wre();
			}
		}
	}

	template<typename T>
	void writeBE(T c, uint_fast8_t n)
	{
		while(n != 0)
		{
			n--;
			b |= ((c >> n) & 1) << (7 - o);
			o++;
			if(o == 8)
			{
				wre();
			}
		}
	}
};

//76543210
class bitWriterL : public bitWriter
{
public:
	bitWriterL(byteWriter &d) : bitWriter(d){}

	template<typename T>
	void writeLE(T c, uint_fast8_t n)
	{
		while(n != 0)
		{
			n--;
			b |= ((c >> n) & 1) << o;
			o++;
			if(o == 8)
			{
				wre();
			}
		}
	}

	template<typename T>
	void writeBE(T c, uint_fast8_t n)
	{
		while(n != 0)
		{
			n--;
			b |= (c & 1) << o;
			c >>= 1;
			o++;
			if(o == 8)
			{
				wre();
			}
		}
	}
};

#endif
