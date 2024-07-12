#ifndef H_DECODE
#define H_DECODE

#include "convert/hex.h"

namespace decode
{
	static bool unchunk(uint8_t* v, std::size_t &n)
	{
		const uint8_t* b = v;
		uint8_t* p = v;
		for(;;)
		{
			std::size_t sz = 0;
			while(n != 0)
			{
				if(*p == '\r')
					break;
				sz = (sz << 4) | convert::hex::Dec::pr_char(*p);
				p++;
				n--;
			}
			if(sz == 0)
			{
				n = v - b;
				return true;
			}
			if(n < sz + 4)
				break;
			n -= sz + 4;

			p += 2;
			std::memmove(v, p, sz);
			v += sz;
			p += sz + 2;
		}
		return false;
	}
}

#endif
