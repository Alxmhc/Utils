#ifndef H_BROTLI
#define H_BROTLI

#include "../bit_reader.h"

namespace compr
{
	class brotli
	{
		static bool wbit(bitReaderL &br, uint_fast8_t &p)
		{
			if(!br.get(p))
				return false;
			if(p == 0)
			{
				p = 16;
				return true;
			}
			if(!br.readBE(3, p))
				return false;
			if(p != 0)
			{
				p += 17;
				return true;
			}
			if(!br.readBE(3, p))
				return false;
			if(p == 0)
			{
				p = 17;
				return true;
			}
			if(p == 1)
				return false;
			p += 8;
			return true;
		}

		static bool read_nbt(bitReaderL &br, uint_fast16_t &t)
		{
			uint_fast8_t n;
			if(!br.get(n))
				return false;
			if(n == 0)
			{
				t = 1;
				return true;
			}
			if(!br.readBE(3, n))
				return false;
			if(!br.readBE(n, t))
				return false;
			t += 1 << n;
			t++;
			return true;
		}
	};
}

#endif
