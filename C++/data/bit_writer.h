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
public:
	bitWriterR(byteWriter &d) : bitWriter(d){}

	template<typename T>
	void writeLE(T c, uint_fast8_t n)
	{
		while(n--)
		{
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
		while(n--)
		{
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
		while(n--)
		{
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
		while(n--)
		{
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
