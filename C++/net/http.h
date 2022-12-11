namespace URL
{
	std::vector<uint8_t> Decode(const char* s, size_t sz)
	{
		std::vector<uint8_t> r;
		r.reserve(sz);
		char t[3] = {};
		for(size_t i = 0; i < sz;)
		{
			if(s[i] != '%')
			{
				r.push_back(s[i]);
				i++;
				continue;
			}
			t[0] = s[i+1];
			t[1] = s[i+2];
			i += 3;
			uint8_t c = static_cast<uint8_t>(strtoul(t, nullptr, 16));
			r.push_back(c);
		}
		r.shrink_to_fit();
		return r;
	}
}

class HTTP_
{
	byteReader* br;
	
	std::string fln;
	std::map<std::string, std::vector<std::string>> hdr;
	size_t dpos;
public:
	bool read(byteReader* b)
	{
		br = b;
		hdr.clear();

		dpos = br->find(bytes("\r\n\r\n"), 4);
		if(dpos == br->get_size())
			return false;
		dpos += 4;

		std::string h;
		br->readN(h, dpos - 2);
		size_t p = h.find("\r\n");
		fln = h.substr(0, p);
		p += 2;

		while(p < h.length())
		{
			auto e = h.find("\r\n", p);
			auto str = h.substr(p, e - p);
			p = e + 2;

			e = str.find(": ");
			if(e == std::string::npos)
				return false;
			std::string k = str.substr(0, e);
			std::transform(k.begin(), k.end(), k.begin(), tolower);
			hdr[k].push_back(str.substr(e + 2));
		}
		return true;
	}

	std::vector<uint8_t> GetData()
	{
		br->set_pos(dpos);
		std::vector<uint8_t> res;
		br->readN(res, br->get_rsize());

		auto k = hdr.find("transfer-encoding");
		if(k != hdr.end())
		{
			if(k->second[0] == "chunked")
			{
				std::vector<uint8_t> tmp;
				bw_array bw(tmp);
				decode::unchunk(res.data(), res.size(), bw);
				res = tmp;
			}
		}
		k = hdr.find("content-encoding");
		if(k != hdr.end())
		{
			if(k->second[0] == "gzip")
			{
				br_array br(res.data(), res.size());
				fl_pr::F_gzip gz;
				gz.read(&br);
				std::vector<uint8_t> tmp;
				bw_array bw(tmp);
				gz.GetData(tmp);
				res = tmp;
			}
		}
		return res;
	}
};
