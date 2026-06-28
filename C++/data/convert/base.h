#ifndef H_BASE
#define H_BASE

#include <cstdint>
#include <string>

namespace convert
{
	namespace base
	{
		static const char* dict_d = "0123456789";
		static const char* dct_h_l = "0123456789abcdef";
		static const char* dct_h_u = "0123456789ABCDEF";

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
