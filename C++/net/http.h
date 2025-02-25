#ifndef H_HTTP
#define H_HTTP

#include <map>
#include <deque>

#include "../data/convert/hex.h"
#include "../data/b.h"
#include "../data/decode.h"
#include "../fl/fl_gzip.h"

namespace URL
{
	static std::vector<uint8_t> Decode(const char* s, std::size_t sz)
	{
		std::vector<uint8_t> r;
		r.reserve(sz);
		for(std::size_t i = 0; i < sz;)
		{
			const char c = s[i];
			i++;
			if(c != '%')
			{
				r.push_back(c);
				continue;
			}
			const uint8_t n = convert::hex::Dec::pr_byte(s + i);
			r.push_back(n);
			i += 2;
		}
		r.shrink_to_fit();
		return r;
	}
}

struct http_header
{
	bool is_out;
	std::string f;
	std::string s;

	std::map<std::string, std::string> m;

	void clear()
	{
		f.clear();
		s.clear();
		m.clear();
	}

	void AddField(const std::string &name, const std::string &val, bool replace = false)
	{
		std::string lname(name);
		for(std::size_t i = 0; i < lname.size(); i++)
		{
			lname[i] = tolower(lname[i]);
		}
		if(replace || m.find(name) == m.end())
		{
			m[lname] = val;
		}
		else
		{
			m[lname] += "; ";
			m[lname] += val;
		}
	}

	bool GetField(const std::string &name, std::string &val) const
	{
		const auto h = m.find(name);
		if(h == m.cend())
			return false;
		val = h->second;
		return true;
	}

	bool data_decode(std::vector<uint8_t> &res) const
	{
		std::string fld;
		if(GetField("transfer-encoding", fld)
		&& fld == "chunked")
		{
			std::size_t sz = res.size();
			if( !decode::unchunk(res.data(), sz) )
				return false;
			res.resize(sz);
		}
		if(GetField("content-encoding", fld))
		{
			if(fld == "gzip")
			{
				br_array br(res.data(), res.size());
				fl_pr::F_gzip gz;
				if( !gz.read(&br) )
					return false;
				std::vector<uint8_t> tmp;
				bw_array bw(tmp);
				if( !gz.GetData(bw) )
					return false;
				res = std::move(tmp);
			}
			else if(fld == "deflate")
			{
				br_array br(res.data(), res.size());
				std::vector<uint8_t> tmp;
				bw_array bw(tmp);
				if( !compr::deflate::Decode(br, bw) )
					return false;
				res = std::move(tmp);
			}
		}
		return true;
	}
};

class HTTP1
{
	byteReader* br;

	http_header hdr;
	std::size_t data_pos;
public:
	static bool parse_hdr(const char* sb, std::size_t sz, http_header &res)
	{
		res.clear();
		const char* se = sb + sz;
		{
			std::vector<const char*> p;
			p.reserve(2);
			const char* f = sb;
			for(; *f != '\r'; f++)
			{
				if(*f == ' ')
				{
					p.push_back(f);
				}
			}
			if(p.size() != 2 || *(f+1) != '\n')
				return false;

			res.is_out = std::memcmp(sb, "HTTP", 4) != 0;
			if(res.is_out)
			{
				res.f.assign(sb, p[0]);
				res.s.assign(p[0]+1, p[1]);
			}
			else
			{
				res.f.assign(p[0]+1, p[1]);
				res.s.assign(p[1]+1, f);
			}
			sb = f + 2;
		}

		static const char* rn = "\r\n";
		static const char* d = ": ";
		while(sb != se)
		{
			const auto p = std::search(sb, se, rn, rn + 2);
			const auto f = std::search(sb, p, d, d + 2);
			if(f == p)
				return false;
			res.AddField(std::string(sb, f), std::string(f + 2, p));
			sb = p + 2;
		}
		return true;
	}

	static std::string to_text(const http_header &hdr)
	{
		std::string res = hdr.is_out ? hdr.f + ' ' + hdr.s + " HTTP/1.1\r\n" : "HTTP/1.1 " + hdr.f + (hdr.s.empty() ? "" : ' ' + hdr.s) + "\r\n";
		for(auto e = hdr.m.cbegin(); e != hdr.m.cend(); ++e)
		{
			res += e->first + ": " + e->second + "\r\n";
		}
		res += "\r\n";
		return res;
	}

	bool read(byteReader* b)
	{
		br = b;
		
		std::string h;
		if(!br->read_string(bytes("\r\n\r\n"), 4, h))
			return false;
		h += "\r\n";
		if(!parse_hdr(h.c_str(), h.length(), hdr))
			return false;

		data_pos = br->get_pos();
		return true;
	}

	bool GetData(std::vector<uint8_t> &data)
	{
		br->set_pos(data_pos);
		br->readN(data, br->get_rsize());
		return hdr.data_decode(data);
	}
};

#endif
