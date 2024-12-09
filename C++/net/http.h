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

	void AddField(const std::string &name, const std::string &val)
	{
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

	bool GetField(const std::string &name, std::string &val) const
	{
		const auto h = m.find(name);
		if(h == m.cend())
			return false;
		val = h->second;
		return true;
	}
};

class HTTP1
{
	byteReader* br;

	http_header hdr;
	std::size_t data_pos;
public:
	bool read(byteReader* b)
	{
		br = b;
		hdr.clear();

		data_pos = br->find(bytes("\r\n\r\n"), 4);
		if(data_pos == br->get_size())
			return false;
		data_pos += 4;

		std::string h;
		br->readN(h, data_pos - 2);
		const char* sb = h.c_str();
		const char* se = sb + h.length();

		const char* rn = "\r\n";
		{
			const auto p = std::search(sb, se, rn, rn + 2);
			hdr.is_out = std::memcmp(sb, "HTTP", 4) != 0;
			const auto p1 = std::find(sb, p, ' ');
			if(p1 == p)
				return false;
			const auto p2 = std::find(p1 + 1, p, ' ');
			if(p2 == p)
				return false;
			if(hdr.is_out)
			{
				hdr.f.assign(sb, p1);
				hdr.s.assign(p1+1, p2);
			}
			else
			{
				hdr.f.assign(p1+1, p2);
				hdr.s.assign(p2+1, p);
			}
			sb = p + 2;
		}

		const char* d = ": ";
		while(sb != se)
		{
			const auto p = std::search(sb, se, rn, rn + 2);
			const auto f = std::search(sb, p, d, d + 2);
			if(f == p)
				return false;
			std::string k(sb, f);
			std::transform(k.begin(), k.end(), k.begin(), tolower);
			hdr.AddField(k, std::string(f + 2, p));
			sb = p + 2;
		}
		return true;
	}

	bool GetData(std::vector<uint8_t> &data)
	{
		br->set_pos(data_pos);
		br->readN(data, br->get_rsize());

		std::string fld;
		if(hdr.GetField("transfer-encoding", fld)
		&& fld == "chunked")
		{
			std::size_t sz = data.size();
			if( !decode::unchunk(data.data(), sz) )
				return false;
			data.resize(sz);
		}
		if(hdr.GetField("content-encoding", fld))
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
	static bool int_decode(br_array &br, uint_fast32_t &res, uint_fast8_t nBit)
	{
		{
			const auto p = (1 << nBit) - 1;
			res &= p;
			if(res != p)
				return true;
		}
		for(uint_fast8_t i = 0; i < 4; i++)
		{
			uint8_t t;
			if( !br.get(t) )
				return false;
			res += static_cast<uint_fast32_t>(t & 0x7f) << (7*i);
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

		uint_fast32_t size = t;
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

	typedef std::pair<std::string, std::string> field;
	static const unsigned char stat_tbl_sz = 61;
	static const field stat_tbl[stat_tbl_sz];

	class h2_tbl
	{
		std::deque<field> tbl;
		std::size_t msz;
	public:
		h2_tbl() : msz(0) {}

		void set_msize(std::size_t nsz)
		{
			msz = nsz;
			if(tbl.size() > msz)
			{
				tbl.resize(msz);
			}
		}

		void addFld(const std::string &name, const std::string &val)
		{
			tbl.emplace_front(field(name, val));
			if(tbl.size() > msz)
			{
				tbl.resize(msz);
			}
		}

		bool get(unsigned int num, std::string &name, std::string &val)
		{
			if(num == 0)
				return false;
			num--;
			if(num < stat_tbl_sz)
			{
				name = stat_tbl[num].first;
				val = stat_tbl[num].second;
				return true;
			}
			num -= stat_tbl_sz;
			if(num >= tbl.size())
				return false;
			name = tbl[num].first;
			val = tbl[num].second;
			return true;
		}
	};

	static bool header_decode(br_array &br, http_header &hdr, h2_tbl &dtbl)
	{
		for(;;)
		{
			uint8_t t;
			if( !br.get(t) )
				break;
			uint_fast32_t id = t;

			std::string name, val;
			if((id & 0x80) != 0)
			{
				if( !int_decode(br, id, 7) )
					return false;
				if(!dtbl.get(id, name, val))
					return false;
			}
			else if((id & 0x40) != 0)
			{
				if(id != 0x40)
				{
					if( !int_decode(br, id, 6) )
						return false;
					if(!dtbl.get(id, name, val))
						return false;
				}
				else
				{
					if( !string_decode(br, name) )
						return false;
				}
				if( !string_decode(br, val) )
					return false;
				dtbl.addFld(name, val);
			}
			else if((id & 0x20) != 0)
			{
				if( !int_decode(br, id, 5) )
					return false;
				dtbl.set_msize(id);
				continue;
			}
			else if((id & 0x10) != 0)
			{
				if(id != 0x10)
				{
					if( !int_decode(br, id, 4) )
						return false;
					if(!dtbl.get(id, name, val))
						return false;
				}
				else
				{
					if( !string_decode(br, name) )
						return false;
				}
				if( !string_decode(br, val) )
					return false;
			}
			else
			{
				if(id != 0)
				{
					if( !int_decode(br, id, 4) )
						return false;
					if(!dtbl.get(id, name, val))
						return false;
				}
				else
				{
					if( !string_decode(br, name) )
						return false;
				}
				if( !string_decode(br, val) )
					return false;
			}
			if(name.empty())
				return false;
			if(name[0] == ':')
			{
				if(name == ":authority")
				{
					hdr.AddField("host", val);
				}
				else if(name == ":method")
				{
					hdr.is_out = true;
					hdr.f = val;
				}
				else if(name == ":path")
				{
					hdr.is_out = true;
					hdr.s = val;
				}
				else if(name == ":status")
				{
					hdr.is_out = false;
					hdr.f = val;
				}
				continue;
			}
			hdr.AddField(name, val);
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

	h2_tbl tbl;
public:
	static bool Is_http2(const uint8_t* data, std::size_t size)
	{
		if(size < hdr_size)
			return false;
		return std::memcmp(data, "\x50\x52\x49\x20\x2a\x20\x48\x54\x54\x50\x2f\x32\x2e\x30\x0d\x0a\x0d\x0a\x53\x4d\x0d\x0a\x0d\x0a", hdr_size) == 0;
	}

	struct frame
	{
		static const unsigned char hdr_size = 9;

		uint_fast8_t type, fl;
		uint32_t id;
		std::vector<uint8_t> buf;

		bool Update(byteReader &br)
		{
			if(buf.size() < hdr_size)
			{
				br.addMx(buf, hdr_size - buf.size());
				if(buf.size() < hdr_size)
					return false;
			}

			uint_fast32_t frlen = buf[0];
			frlen = (frlen << 8) | buf[1];
			frlen = (frlen << 8) | buf[2];
			frlen += hdr_size;
			if(buf.size() < frlen)
			{
				br.addMx(buf, frlen - buf.size());
				if(buf.size() < frlen)
					return false;
			}

			type = buf[3];
			fl = buf[4];
			id = bconv<1, 4, endianness::BIG_ENDIAN>::pack(buf.data() + 5);
			id &= 0x7fffffff;

			return true;
		}

		void Clear()
		{
			buf.clear();
		}
	};

	struct pack
	{
		http_header hdr;
		std::vector<uint8_t> bd;
		bool h_fin, d_fin;

		pack() : h_fin(false), d_fin(false) {}
	};

	bool frame_decode(const frame &fr, pack* p)
	{
		br_array br(fr.buf.data(), fr.buf.size());
		br.skip(frame::hdr_size);
		switch(fr.type)
		{
		case 4:
		{
			uint8_t t;
			if(!br.get(t) || t != 0)
				break;
			if(!br.get(t) || t != 1)
				break;
			uint32_t nsz;
			if(br.readC<4, endianness::BIG_ENDIAN>(nsz))
			{
				tbl.set_msize(nsz);
			}
			break;
		}
		case 0:
		{
			if((fr.fl & 0x08) != 0)
			{
				if( !unpad(br) )
					return false;
			}
			br.addN(p->bd, br.get_rsize());
			p->d_fin = (fr.fl & 0x01) != 0;
			break;
		}
		case 1:
		{
			if((fr.fl & 0x08) != 0)
			{
				if( !unpad(br) )
					return false;
			}
			if((fr.fl & 0x20) != 0)
			{
				if( !br.skip(5) )
					return false;
			}
			if( !header_decode(br, p->hdr, tbl) )
				return false;
			p->h_fin = (fr.fl & 0x04) != 0;
			break;
		}
		case 9:
		{
			if( !header_decode(br, p->hdr, tbl) )
				return false;
			p->h_fin = (fr.fl & 0x04) != 0;
			break;
		}
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
