#ifndef H_PEM
#define H_PEM

#include "../byte_reader.h"
#include "base64.h"

class pem
{
public:
	typedef std::pair<std::string, std::vector<uint8_t>> cert;

	static bool Parse(byteReader &br, cert &res)
	{
		std::string st;
		st.reserve(64);
		if( !br.read_string('\n', st) )
			return false;
		if(st.empty() || st[0] != '-')
			return false;
		{
			auto p0 = st.find(' ') + 1;
			auto p1 = st.find('-', p0);
			res.first.assign(st.cbegin() + p0, st.cbegin() + p1);
		}

		res.second.clear();
		bw_array bw(res.second);
		convert::base64::Decoder d(bw);
		for(;;)
		{
			br.read_string('\n', st);
			if(st.empty())
				return false;
			if(st[0] == '-')
				break;
			auto sz = st.size();
			if(st[sz-1] == '\r')
			{
				sz--;
			}
			while(st[sz-1] == '=')
			{
				sz--;
			}
			d.writeN(reinterpret_cast<const uint8_t*>(st.c_str()), sz);
		}
		d.Fin();
		return true;
	}

	static std::vector<cert> Read(byteReader &br)
	{
		std::vector<cert> data;

		cert tmp;
		while( pem::Parse(br, tmp) )
		{
			data.push_back(tmp);
		}

		return data;
	}
};

#endif
