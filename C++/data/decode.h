#ifndef H_DECODE
#define H_DECODE

#include <cstdint>
#include <algorithm>

namespace decode
{
	bool unchunk(uint8_t* v, std::size_t &n)
	{
		std::size_t nsz = 0;
		for(;;)
		{
			const auto sz = strtoul(reinterpret_cast<const char*>(v), nullptr, 16);
			if(sz == 0)
			{
				n = nsz;
				return true;
			}
			nsz += sz;

			auto e = std::find(v + 2, v + n, '\n');
			if(e == v + n)
				break;
			e += 1;
			n -= e - v;
			std::memmove(v, e, n);
			v += sz;

			if(v[1] != '\n' || sz > n)
				break;
			n -= sz;
		}
		return false;
	}
}

#endif
