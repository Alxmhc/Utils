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

class HTTP_ : public cont_1
{
	std::string fln;
	std::map<std::string, std::vector<std::string>> hdr;
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
			hdr[k].push_back(std::string(f + 2, p));
			sb = p + 2;
		}
		return true;
	}

	bool GetData(std::vector<uint8_t> &data)
	{
		getData(data);

		auto k = hdr.find("transfer-encoding");
		if(k != hdr.end())
		{
			if(k->second[0] == "chunked")
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
			if(k->second[0] == "gzip")
			{
				br_array br(data.data(), data.size());
				fl_pr::F_gzip gz;
				gz.read(&br);
				std::vector<uint8_t> tmp;
				if( !gz.GetData(tmp) )
					return false;
				data = std::move(tmp);
			}
		}
		return true;
	}
};
