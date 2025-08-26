#ifndef H_PEM
#define H_PEM

#include <fstream>

#include "../str.h"
#include "../data/convert/base64.h"

namespace fl_pr
{
	class pem
	{
	public:
		typedef std::pair<std::string, std::vector<uint8_t>> cert;

		static std::vector<cert> read(const char* fname)
		{
			std::vector<cert> res;
			std::ifstream fl(fname);
			for(;;)
			{
				std::string name;
				if(!std::getline(fl, name))
					break;
				if(!is_b(name, "-----BEGIN ")
				|| !is_e(name, "-----"))
					break;
				name = name.substr(11, name.length() - 16);

				std::vector<uint8_t> d;
				bw_array bw(d);
				convert::base64::Decoder de(bw);
				std::string tmp;
				while(std::getline(fl, tmp))
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
					std::getline(fl, tmp);
					break;
				}
				if(tmp != "-----END " + name + "-----")
					break;
				de.Fin();
				res.push_back(cert(name, d));
			}
			return res;
		}
	};
}

#endif
