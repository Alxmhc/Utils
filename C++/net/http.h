#ifndef H_HTTP
#define H_HTTP

#include <map>

#include "../str.h"
#include "../data/convert/hex.h"
#include "../data/decode.h"
#include "../fl/fl_gzip.h"

class dict_reader
{
	const std::string d, e;
public:
	dict_reader(const char* f, const char* p) : d(f), e(p) {}

	bool nxt(const char* &sb, const char* se, std::pair<std::string, std::string> &res) const
	{
		const auto f = std::search(sb, se, d.cbegin(), d.cend());
		const auto p = std::search(sb, f, e.cbegin(), e.cend());
		if (p == f)
			return false;
		res.first.assign(sb, p);
		res.second.assign(p + e.length(), f);
		sb = f != se ? f + d.length() : se;
		return true;
	}
};

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
		clear();
		static const dict_reader rd("\r\n", ": ");
		std::pair<std::string, std::string> e;
		while(cb != ce)
		{
			if (!rd.nxt(cb, ce, e))
				return false;
			AddField(e.first, e.second);
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
			const uint8_t n = convert::hex::Decoder::pr_byte(s + i);
			r.push_back(n);
			i += 2;
		}
		r.shrink_to_fit();
		return r;
	}

	static bool Parse_params(const std::string &s, std::map<std::string, std::string> &par)
	{
		const char* cb = s.c_str();
		const char* ce = cb + s.length();
		par.clear();
		static const dict_reader rd("&", "=");
		std::pair<std::string, std::string> e;
		while(cb != ce)
		{
			if (!rd.nxt(cb, ce, e))
				return false;
			par[e.first] = e.second;
		}
		return true;
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

	static bool parse_l(const std::string &s, http_header &res)
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

	bool Decode(byteReader &brd, byteWriter &bw) const
	{
		std::string fld;
		if (hdr.h.GetField("content-encoding", fld))
		{
			if (fld == "gzip")
			{
				fl_pr::F_gzip gz;
				if (!gz.read(brd))
					return false;
				if (!gz.GetData(bw))
					return false;
				return true;
			}
			else if (fld == "deflate")
			{
				if (!compr::deflate::Decode(brd, bw))
					return false;
				return true;
			}
		}
		std::vector<uint8_t> data;
		brd.readN(data, brd.get_rsize());
		bw.writeN(data.data(), data.size());
		return true;
	}
public:
	static bool Read_hdr(byteReader &br, std::string &s)
	{
		const auto p = br.find(bytes("\r\n\r\n"), 4);
		if (p == br.get_rsize())
			return false;
		br.readN(s, p + 2);
		br.skip(2);
		return true;
	}

	static bool Hdr_From_Text(const std::string &s, http_header &h)
	{
		const auto p = s.find("\r\n");
		if (!parse_l(s.substr(0, p), h))
			return false;
		if (!h.h.From_Text(s.c_str() + p + 2, s.c_str() + s.length()))
			return false;
		return true;
	}

	static bool Read_hdr(byteReader &br, http_header &hdr)
	{
		std::string s;
		if (!Read_hdr(br, s))
			return false;
		if(!Hdr_From_Text(s, hdr))
			return false;
		return true;
	}

	static std::string Hdr_To_Text(const http_header &h)
	{
		std::string res = h.is_out ? h.f + ' ' + h.s + " HTTP/1.1\r\n" : "HTTP/1.1 " + h.f + (h.s.empty() ? "" : ' ' + h.s) + "\r\n";
		res += h.h.To_Text() + "\r\n";
		return res;
	}

	bool read(byteReader &b)
	{
		br = &b;
		if (!Read_hdr(*br, hdr))
			return false;
		data_pos = br->get_pos();
		return true;
	}

	const http_header* Get_Header() const
	{
		return &hdr;
	}

	bool Get_Data(byteWriter &bw)
	{
		std::string fld;
		if(hdr.h.GetField("content-length", fld))
		{
			const auto sz = std::stoul(fld);
			if (sz == 0)
				return true;
			br->set_pos(data_pos);
			if(!br->set_rsize(sz))
				return false;
			if (!Decode(*br, bw))
				return false;
			return true;
		}
		if(hdr.h.GetField("transfer-encoding", fld) && fld == "chunked")
		{
			br->set_pos(data_pos);
			std::vector<uint8_t> data;
			data.reserve(br->get_rsize());
			bw_vector bwd(data);
			if(!decode::chunk_read(*br, bwd))
				return false;
			br_array brd(data.data(), data.size());
			if (!Decode(brd, bw))
				return false;
			return true;
		}
		return true;
	}
};

#endif
