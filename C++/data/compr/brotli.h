#ifndef H_BROTLI
#define H_BROTLI

#include "../bit_reader.h"

namespace compr
{
	class brotli
	{
	public:
		static bool wbit(bitReaderL &br, uint_fast8_t &r)
		{
			if(!br.get(r))
				return false;
			if(r == 0)
			{
				r = 16;
				return true;
			}
			if(!br.readBE(3, r))
				return false;
			if(r != 0)
			{
				r += 17;
				return true;
			}
			if(!br.readBE(3, r))
				return false;
			if(r != 0)
			{
				if(r == 1)
					return false;
				r += 8;
				return true;
			}
			r = 17;
			return true;
		}
	};
}

#endif
