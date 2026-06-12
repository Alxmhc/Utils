#ifndef H_BASE
#define H_BASE

#include <cstdint>
#include <string>

namespace convert
{
	namespace base
	{
		namespace Enc
		{
			template<uint_fast8_t BASE>
			static std::string pr_num(std::size_t n, const char* dict)
			{
				if(n == 0)
					return std::string(dict, dict + 1);
				std::string res;
				while(n != 0)
				{
					res += dict[n % BASE];
					n /= BASE;
				}
				std::reverse(res.begin(), res.end());
				return res;
			}
		}
	}
}

#endif
