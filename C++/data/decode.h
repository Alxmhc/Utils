#ifndef H_DECODE
#define H_DECODE

#include "convert/hex.h"
#include "byte_reader.h"
#include "byte_writer.h"

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
				const uint8_t c = convert::hex::Dec::pr_char(*p);
				if(c == 16)
					return false;
				sz = (sz << 4) | c;
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

	static bool chunk_read(byteReader &br, byteWriter &bw)
	{
		for(;;)
		{
			std::size_t sz = 0;
			for(;;)
			{
				uint8_t c;
				if(!br.get(c))
					return false;
				if(c == '\r')
					break;
				c = convert::hex::Dec::pr_char(c);
				if(c == 16)
					return false;
				sz = (sz << 4) | c;
			}
			if(sz == 0)
				return true;
			if(!br.skip(1))
				return false;
			{
				std::vector<uint8_t> tmp;
				if(!br.readN(tmp, sz))
					return false;
				bw.writeN(tmp.data(), sz);
			}
			if(!br.skip(2))
				return false;
		}
	}
}

#endif
