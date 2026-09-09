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

		namespace Dec
		{
			template<uint_fast8_t BASE>
			bool pr_char(char c, uint8_t &r)
			{
				switch (c)
				{
				case '0':
					r = 0;
					break;
				case '1':
					r = 1;
					break;
				case '2':
					r = 2;
					break;
				case '3':
					r = 3;
					break;
				case '4':
					r = 4;
					break;
				case '5':
					r = 5;
					break;
				case '6':
					r = 6;
					break;
				case '7':
					r = 7;
					break;
				case '8':
					r = 8;
					break;
				case '9':
					r = 9;
					break;
				case 'a':
				case 'A':
					r = 10;
					break;
				case 'b':
				case 'B':
					r = 11;
					break;
				case 'c':
				case 'C':
					r = 12;
					break;
				case 'd':
				case 'D':
					r = 13;
					break;
				case 'e':
				case 'E':
					r = 14;
					break;
				case 'f':
				case 'F':
					r = 15;
					break;
				default:
					return false;
				}
				return r < BASE;
			}

			template<uint_fast8_t N, typename T>
			static bool pr_num(const char* s, uint_fast8_t n, T &res)
			{
				res = 0;
				for (uint_fast8_t i = 0; i < n; i++)
				{
					uint8_t k;
					if (!pr_char<N>(s[i], k))
						return false;
					res = (res * N) + k;
				}
				return true;
			}
		}
	}
}

#endif
