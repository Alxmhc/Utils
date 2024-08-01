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

class HTTP1 : public cont_1
{
	std::string fln;
	std::map<std::string, std::string> hdr;
public:
	bool read(byteReader* b)
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
			if(hdr.find(k) == hdr.end())
			{
				hdr[k] = std::string(f + 2, p);
			}
			else
			{
				hdr[k] += "; ";
				hdr[k] += std::string(f + 2, p);
			}
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
			if(k->second == "chunked")
			{
				std::size_t sz = data.size();
				if( !decode::unchunk(data.data(), sz) )
					return false;
				data.resize(sz);
			}
		}
		k = hdr.find("content-encoding");
		if(k != hdr.end())
		{
			const auto type = k->second;
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
		return h->second;
	}
};

static const unsigned char http2_hcode[257] = {
	13, 23, 28, 28, 28, 28, 28, 28, 28, 24, 30, 28, 28, 30, 28, 28,
	28, 28, 28, 28, 28, 28, 30, 28, 28, 28, 28, 28, 28, 28, 28, 28,
	 6, 10, 10, 12, 13,  6,  8, 11, 10, 10,  8, 11,  8,  6,  6,  6,
	 5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  7,  8, 15,  6, 12, 10,
	13,  6,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
	 7,  7,  7,  7,  7,  7,  7,  7,  8,  7,  8, 13, 19, 13, 14,  6,
	15,  5,  6,  5,  6,  5,  6,  6,  6,  5,  7,  7,  6,  6,  6,  5,
	 6,  7,  6,  5,  5,  6,  7,  7,  7,  7,  7, 15, 11, 14, 13, 28,
	20, 22, 20, 20, 22, 22, 22, 23, 22, 23, 23, 23, 23, 23, 24, 23,
	24, 24, 22, 23, 24, 23, 23, 23, 23, 21, 22, 23, 22, 23, 23, 24,
	22, 21, 20, 22, 22, 23, 23, 21, 23, 22, 22, 24, 21, 22, 23, 23,
	21, 21, 22, 21, 23, 22, 23, 23, 20, 22, 22, 22, 23, 22, 22, 23,
	26, 26, 20, 19, 22, 23, 22, 25, 26, 26, 26, 27, 27, 26, 24, 25,
	19, 21, 26, 27, 27, 26, 27, 24, 21, 21, 26, 26, 28, 27, 27, 27,
	20, 24, 20, 21, 22, 21, 21, 23, 22, 22, 25, 25, 24, 24, 26, 23,
	26, 27, 26, 26, 27, 27, 27, 27, 27, 28, 27, 27, 27, 27, 27, 26,
	30};
static const hTree<unsigned int> http2_htr(http2_hcode, 257);

class HTTP2
{
	template<typename T>
	static bool int_decode(br_array &br, T &res, uint_fast8_t nBit)
	{
		{
			const auto p = (1 << nBit) - 1;
			res &= p;
			if(res != p)
				return true;
		}
		for(uint_fast8_t i = 0; i < sizeof(T); i++)
		{
			uint8_t t;
			if( !br.get(t) )
				return false;
			res += static_cast<T>(t & 0x7f) << (7*i);
			if((t & 0x80) == 0)
				return true;
		}
		return false;
	}

	static bool string_decode(br_array &br, std::string &out_str)
	{
		uint8_t t;
		if( !br.get(t) )
			return false;
		const bool is_huff = (t & 0x80) != 0;

		uint_fast16_t size = t;
		if( !int_decode(br, size, 7) )
			return false;

		if( !is_huff )
			return br.readN(out_str, size);

		if( !br.set_rsize(size) )
			return false;
		out_str.clear();
		bitReaderR br1(br);
		for(;;)
		{
			unsigned int r;
			if(!btree_decode(&http2_htr, br1, r))
				break;
			if(r == 256)
				return false;
			out_str.push_back(r);
		}
		if(br.get_rsize() != 0)
			return false;
		br.reset_size();
		return true;
	}

	static const unsigned char hdr_size = 24;
	static const unsigned char fr_hdr_size = 9;

	typedef std::pair<std::string, std::string> field;
	static const unsigned char stat_tbl_sz = 61;
	static const field stat_tbl[stat_tbl_sz];
	std::size_t dyn_tbl_msz;
	std::deque<field> dyn_tbl;

	static field tbl_get(unsigned int num, const std::deque<field> &d_fld)
	{
		if(num == 0)
			return field("", "");
		num--;
		if(num < stat_tbl_sz)
			return stat_tbl[num];
		num -= stat_tbl_sz;
		if(num >= d_fld.size())
			return field("", "");
		return d_fld[num];
	}

	bool header_decode(br_array &br, std::vector<field> &hdr)
	{
		for(;;)
		{
			uint8_t t;
			if( !br.get(t) )
				break;
			uint_fast32_t id = t;

			field fld;
			if((id & 0x80) != 0)
			{
				if( !int_decode(br, id, 7) )
					return false;
				fld = tbl_get(id, dyn_tbl);
			}
			else if((id & 0x40) != 0)
			{
				if(id != 0x40)
				{
					if( !int_decode(br, id, 6) )
						return false;
					fld = tbl_get(id, dyn_tbl);
				}
				else
				{
					if( !string_decode(br, fld.first) )
						return false;
				}
				if( !string_decode(br, fld.second) )
					return false;
				dyn_tbl.emplace_front(fld);
				if(dyn_tbl.size() > dyn_tbl_msz)
				{
					dyn_tbl.resize(dyn_tbl_msz);
				}
			}
			else if((id & 0x20) != 0)
			{
				if( !int_decode(br, id, 5) )
					return false;
				dyn_tbl_msz = id < 4096 ? id : 4096;
				continue;
			}
			else if((id & 0x10) != 0)
			{
				if(id != 0x10)
				{
					if( !int_decode(br, id, 4) )
						return false;
					fld = tbl_get(id, dyn_tbl);
				}
				else
				{
					if( !string_decode(br, fld.first) )
						return false;
				}
				if( !string_decode(br, fld.second) )
					return false;
			}
			else
			{
				if(id != 0)
				{
					if( !int_decode(br, id, 4) )
						return false;
					fld = tbl_get(id, dyn_tbl);
				}
				else
				{
					if( !string_decode(br, fld.first) )
						return false;
				}
				if( !string_decode(br, fld.second) )
					return false;
			}
			if(fld.first.empty())
				return false;
			hdr.push_back(fld);
		}
		return true;
	}

	static bool unpad(br_array &br)
	{
		uint8_t pad;
		if( !br.get(pad) )
			return false;
		if( br.get_rsize() < pad )
			return false;
		br.set_size(br.get_size() - pad);
		return true;
	}
public:
	struct pack
	{
		std::vector<field> hdr;
		std::vector<uint8_t> bd;
		bool h_fin, d_fin;

		pack() : h_fin(false), d_fin(false) {}
	};

	HTTP2() : dyn_tbl_msz(4096) {}

	static bool Is_http2(const uint8_t* data, std::size_t size)
	{
		if(size < hdr_size)
			return false;
		return std::memcmp(data, "\x50\x52\x49\x20\x2a\x20\x48\x54\x54\x50\x2f\x32\x2e\x30\x0d\x0a\x0d\x0a\x53\x4d\x0d\x0a\x0d\x0a", hdr_size) == 0;
	}

	bool frame_decode(br_array &br, uint_fast8_t t, uint_fast8_t f, pack* p)
	{
		switch(t)
		{
		case 0:
		{
			if((f & 0x08) != 0)
			{
				if( !unpad(br) )
					return false;
			}
			br.addN(p->bd, br.get_rsize());
			p->d_fin = (f & 0x01) != 0;
			break;
		}
		case 1:
		{
			if((f & 0x08) != 0)
			{
				if( !unpad(br) )
					return false;
			}
			if((f & 0x20) != 0)
			{
				if( !br.skip(5) )
					return false;
			}
			if( !header_decode(br, p->hdr) )
				return false;
			p->h_fin = (f & 0x04) != 0;
			break;
		}
		case 9:
		{
			if( !header_decode(br, p->hdr) )
				return false;
			p->h_fin = (f & 0x04) != 0;
			break;
		}
		}
		return true;
	}

	bool buf_read(byteReader &br, std::vector<uint8_t> &buf) const
	{
		if(buf.size() < fr_hdr_size)
		{
			br.addMx(buf, fr_hdr_size - buf.size());
			if(buf.size() < fr_hdr_size)
				return false;
		}

		uint_fast32_t frlen = buf[0];
		frlen = (frlen << 8) | buf[1];
		frlen = (frlen << 8) | buf[2];
		frlen += fr_hdr_size;
		if(buf.size() < frlen)
		{
			br.addMx(buf, frlen - buf.size());
			if(buf.size() < frlen)
				return false;
		}
		return true;
	}
};

const HTTP2::field HTTP2::stat_tbl[] = {
	field(":authority",""),
	field(":method","GET"),
	field(":method","POST"),
	field(":path","/"),
	field(":path","/index.html"),
	field(":scheme","http"),
	field(":scheme","https"),
	field(":status","200"),
	field(":status","204"),
	field(":status","206"),
	field(":status","304"),
	field(":status","400"),
	field(":status","404"),
	field(":status","500"),
	field("accept-charset",""),
	field("accept-encoding","gzip, deflate"),
	field("accept-language",""),
	field("accept-ranges",""),
	field("accept",""),
	field("access-control-allow-origin",""),
	field("age",""),
	field("allow",""),
	field("authorization",""),
	field("cache-control",""),
	field("content-disposition",""),
	field("content-encoding",""),
	field("content-language",""),
	field("content-length",""),
	field("content-location",""),
	field("content-range",""),
	field("content-type",""),
	field("cookie",""),
	field("date",""),
	field("etag",""),
	field("expect",""),
	field("expires",""),
	field("from",""),
	field("host",""),
	field("if-match",""),
	field("if-modified-since",""),
	field("if-none-match",""),
	field("if-range",""),
	field("if-unmodified-since",""),
	field("last-modified",""),
	field("link",""),
	field("location",""),
	field("max-forwards",""),
	field("proxy-authenticate",""),
	field("proxy-authorization",""),
	field("range",""),
	field("referer",""),
	field("refresh",""),
	field("retry-after",""),
	field("server",""),
	field("set-cookie",""),
	field("strict-transport-security",""),
	field("transfer-encoding",""),
	field("user-agent",""),
	field("vary",""),
	field("via",""),
	field("www-authenticate","")
};

#endif
