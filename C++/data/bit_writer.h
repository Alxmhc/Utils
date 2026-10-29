#ifndef H_BIT_WRITER
#define H_BIT_WRITER

#include "byte_writer.h"

class bitWriter
{
protected:
	byteWriter* w;
	uint_fast8_t o, b;

	bitWriter(byteWriter &d) : w(&d), o(0), b(0) {}

	void wre()
	{
		w->write(b);
		b = 0;
		o = 0;
	}

	void wr(uint_fast8_t c)
	{
		b |= c;
		o++;
		if (o == 8)
		{
			wre();
		}
	}
public:
	void pad0()
	{
		if(o != 0)
		{
			wre();
		}
	}
};

//01234567
class bitWriterR : public bitWriter
{
	void write1(uint_fast8_t c)
	{
		wr(c << (7 - o));
	}
public:
	bitWriterR(byteWriter &d) : bitWriter(d){}

	template<typename T>
	void writeLE(T c, uint_fast8_t n)
	{
		while(n--)
		{
			write1(c & 1);
			c >>= 1;
		}
	}

	template<typename T>
	void writeBE(T c, uint_fast8_t n)
	{
		while(n--)
		{
			write1((c >> n) & 1);
		}
	}
};

//76543210
class bitWriterL : public bitWriter
{
	void write1(uint_fast8_t c)
	{
		wr(c << o);
	}
public:
	bitWriterL(byteWriter &d) : bitWriter(d){}

	template<typename T>
	void writeLE(T c, uint_fast8_t n)
	{
		while(n--)
		{
			write1((c >> n) & 1);
		}
	}

	template<typename T>
	void writeBE(T c, uint_fast8_t n)
	{
		while(n--)
		{
			write1(c & 1);
			c >>= 1;
		}
	}
};

#endif
