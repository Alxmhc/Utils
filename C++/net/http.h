#ifndef H_HTTP
#define H_HTTP

#include <map>

#include "../data/convert/hex.h"
#include "../data/b.h"
#include "../data/decode.h"
#include "../fl/fl_gzip.h"

namespace URL
{
	std::vector<uint8_t> Decode(const char* s, std::size_t sz)
	{
		std::vector<uint8_t> r;
		r.reserve(sz);
		for(std::size_t i = 0; i < sz;)
		{
			if(s[i] != '%')
			{
				r.push_back(s[i]);
				i++;
				continue;
			}
			uint8_t c = (HexDecodeChar(s[i+1]) << 4) | HexDecodeChar(s[i+2]);
			r.push_back(c);
			i += 3;
		}
		r.shrink_to_fit();
		return r;
	}
}

class HTTP_ : public cont_1
{
	std::string fln;
	std::map<std::string, std::vector<std::string>> hdr;
public:
	bool read_v1(byteReader* b)
	{
		br = b;
		hdr.clear();

		data_pos = br->find(bytes("\r\n\r\n"), 4);
		if(data_pos == br->get_size())
			return false;
		data_pos += 4;
		data_size = br->get_size() - data_pos;

		std::string h;
		br->readN(h, data_pos - 2);
		const char* sb = h.c_str();
		const char* se = sb + h.length();

		const char* rn = "\r\n";
		{
			const auto p = std::search(sb, se, rn, rn + 2);
			fln.assign(sb, p);
			sb = p + 2;
		}

		const char* d = ": ";
		while(sb != se)
		{
			const auto p = std::search(sb, se, rn, rn + 2);
			const auto f = std::search(sb, p, d, d + 2);
			if(f == p)
			{
				hdr.clear();
				return false;
			}
			std::string k(sb, f);
			std::transform(k.begin(), k.end(), k.begin(), tolower);
			hdr[k].push_back(std::string(f + 2, p));
			sb = p + 2;
		}
		return true;
	}

	bool GetData(std::vector<uint8_t> &data)
	{
		Init();
		br->readN(data, br->get_rsize());

		auto k = hdr.find("transfer-encoding");
		if(k != hdr.end())
		{
			if(k->second.back() == "chunked")
			{
				std::vector<uint8_t> tmp;
				bw_array bw(tmp);
				if( !decode::unchunk(data.data(), data.size(), bw) )
					return false;
				data = std::move(tmp);
			}
		}
		k = hdr.find("content-encoding");
		if(k != hdr.end())
		{
			const auto type = k->second.back();
			if(type == "gzip")
			{
				br_array br(data.data(), data.size());
				fl_pr::F_gzip gz;
				if( !gz.read(&br) )
					return false;
				std::vector<uint8_t> tmp;
				bw_array bw(tmp);
				if( !gz.GetData(bw) )
					return false;
				data = std::move(tmp);
			}
			else if(type == "deflate")
			{
				br_array br(data.data(), data.size());
				std::vector<uint8_t> tmp;
				bw_array bw(tmp);
				if( !compr::deflate::Decode(br, bw) )
					return false;
				data = std::move(tmp);
			}
		}
		return true;
	}

	std::string GetField1(const std::string &name) const
	{
		const auto h = hdr.find(name);
		if(h == hdr.cend())
			return "";
		return h->second.back();
	}
};

#endif
