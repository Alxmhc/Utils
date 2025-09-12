#ifndef H_PEM
#define H_PEM

#include <sstream>
#include <fstream>

#include "../../str.h"
#include "base64.h"

namespace convert
{
	class PEM
	{
		typedef std::pair<std::string, std::vector<uint8_t>> crt;

		static bool read_1(std::istream &st, crt &res)
		{
			if(!std::getline(st, res.first))
				return false;
			if(!is_b(res.first, "-----BEGIN ")
			|| !is_e(res.first, "-----"))
				return false;
			res.first = res.first.substr(11, res.first.length() - 16);

			bw_array bw(res.second);
			convert::base64::Decoder de(bw);
			std::string tmp;
			while(std::getline(st, tmp))
			{
				if(tmp.empty() || tmp[0] == '-')
					break;
				auto sz = tmp.size();
				if(tmp[sz-1] != '=')
				{
					de.writeS(tmp.c_str(), sz);
					continue;
				}
				sz--;
				while(sz != 0 && tmp[sz-1] == '=')
				{
					sz--;
				}
				de.writeS(tmp.c_str(), sz);
				std::getline(st, tmp);
				break;
			}
			if(tmp != "-----END " + res.first + "-----")
				return false;
			de.Fin();
			return true;
		}

		static std::vector<crt> read_s(std::istream &st)
		{
			std::vector<crt> res;
			crt tmp;
			while(read_1(st, tmp))
			{
				res.push_back(std::move(tmp));
			}
			return res;
		}
	public:
		static std::vector<crt> read_string(const char* str)
		{
			std::istringstream s(str);
			return read_s(s);
		}
		static std::vector<crt> read_file(const char* fname)
		{
			std::ifstream fl(fname);
			return read_s(fl);
		}
	};
}

#endif
