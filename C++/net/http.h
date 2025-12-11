#ifndef H_HTTP
#define H_HTTP

#include <map>

#include "../str.h"
#include "../data/convert/hex.h"
#include "../data/decode.h"
#include "../fl/fl_gzip.h"

class net_header
{
	std::map<std::string, std::string> m;
public:
	void clear()
	{
		m.clear();
	}

	void AddField(std::string name, const std::string &val)
	{
		str_lower(name);
		if(m.find(name) == m.end())
		{
			m[name] = val;
		}
		else
		{
			m[name] += "; ";
			m[name] += val;
		}
	}

	bool GetField(std::string name, std::string &val) const
	{
		str_lower(name);
		const auto h = m.find(name);
		if(h == m.cend())
			return false;
		val = h->second;
		return true;
	}

	bool From_Text(const char* cb, const char* ce)
	{
		static const char* rn = "\r\n";
		static const char* d = ": ";
		while(cb != ce)
		{
			const auto p = std::search(cb, ce, rn, rn + 2);
			const auto f = std::search(cb, p, d, d + 2);
			if(f == p)
				return false;
			AddField(std::string(cb, f), std::string(f + 2, p));
			cb = p + 2;
		}
		return true;
	}

	std::string To_Text() const
	{
		std::string res;
		for(auto e = m.cbegin(); e != m.cend(); ++e)
		{
			res += e->first + ": " + e->second + "\r\n";
		}
		return res;
	}
};

struct URL
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
};

struct http_header
{
	bool is_out;
	std::string f;
	std::string s;

	net_header h;

	void clear()
	{
		f.clear();
		s.clear();
		h.clear();
	}
};

class HTTP1
{
	byteReader* br;

	http_header hdr;
	std::size_t data_pos;

	static bool parse_s(const std::string &s, http_header &res)
	{
		const auto p1 = s.find(' ');
		if(p1 == std::string::npos)
			return false;
		const auto p2 = s.find(' ', p1 + 1);
		if(p2 == std::string::npos)
			return false;

		res.is_out = !is_b(s, "HTTP/");
		if(res.is_out)
		{
			res.f = s.substr(0, p1);
			res.s = s.substr(p1 + 1, p2 - p1 - 1);
		}
		else
		{
			res.f = s.substr(p1 + 1, p2 - p1 - 1);
			res.s = s.substr(p2 + 1);
		}

		return true;
	}
public:
	bool read(byteReader* b)
	{
		br = b;
		hdr.clear();
		
		std::string h;
		if(!br->read_string(bytes("\r\n\r\n"), 4, h))
			return false;
		h += "\r\n";

		const auto p = h.find("\r\n");
		if(!parse_s(h.substr(0, p), hdr))
			return false;
		const auto s = h.c_str();
		if(!hdr.h.From_Text(s + p + 2, s + h.length()))
			return false;

		data_pos = br->get_pos();
		return true;
	}

	const http_header* Get_Header() const
	{
		return &hdr;
	}

	std::string Get_Hdr_Text() const
	{
		std::string res = hdr.is_out ? hdr.f + ' ' + hdr.s + " HTTP/1.1\r\n" : "HTTP/1.1 " + hdr.f + (hdr.s.empty() ? "" : ' ' + hdr.s) + "\r\n";
		res += hdr.h.To_Text() + "\r\n";
		return res;
	}

	bool Get_Data(std::vector<uint8_t> &data)
	{
		data.clear();
		br->set_pos(data_pos);

		std::string fld;
		if(hdr.h.GetField("content-length", fld))
		{
			const auto sz = std::stoul(fld);
			if(!br->readN(data, sz))
				return false;
		}
		else if(hdr.h.GetField("transfer-encoding", fld) && fld == "chunked")
		{
			bw_array bw(data);
			if(!decode::chunk_read(*br, bw))
				return false;
		}
		else
			return true;

		if(hdr.h.GetField("content-encoding", fld))
		{
			if(fld == "gzip")
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
			else if(fld == "deflate")
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
};

#endif
