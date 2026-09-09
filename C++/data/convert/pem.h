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
		std::istream &st;
	protected:
		PEM(std::istream &s) : st(s){}
	public:
		typedef std::pair<std::string, std::vector<uint8_t>> crt;

		bool read(crt &res)
		{
			res.first.clear();
			res.second.clear();

			if(!std::getline(st, res.first))
				return false;
			if(!is_b(res.first, "-----BEGIN ")
			|| !is_e(res.first, "-----"))
				return false;
			res.first = res.first.substr(11, res.first.length() - 16);

			bw_vector bw(res.second);
			convert::base64::Decoder de(bw);
			std::string tmp;
			while(std::getline(st, tmp))
			{
				if(tmp.empty() || tmp.front() == '-')
					break;
				if(tmp.back() != '=')
				{
					de.writeS(tmp);
					continue;
				}
				const auto p = tmp.find_last_not_of('=');
				if (p != std::string::npos)
				{
					de.writeS(tmp, p + 1);
				}
				std::getline(st, tmp);
				break;
			}
			if(tmp != "-----END " + res.first + "-----")
				return false;
			de.Fin();
			return true;
		}
	};

	class PEM_string : public PEM
	{
		std::istringstream s;
	public:
		PEM_string() : PEM(s){}

		void open(const char* str)
		{
			s.clear();
			s.str(str);
		}
	};

	class PEM_file : public PEM
	{
		std::ifstream s;
	public:
		PEM_file() : PEM(s){}

		void open(const char* fname)
		{
			s.close();
			s.open(fname);
		}
	};
}

#endif
