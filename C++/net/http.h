class HTTP_
{
	byteReader* br;
	size_t p1;
	size_t p2;
public:
	typedef std::map<std::string, std::vector<std::string>> hdr_type;

	bool read(byteReader* b)
	{
		br = b;
		p1 = br->find(bytes("\r\n"), 2);
		p2 = br->find(bytes("\r\n\r\n"), 4);
		return true;
	}

	std::string get_fl()
	{
		br->set_pos(0);
		std::string fl;
		br->readN(fl, p1);
		return fl;
	}

	static hdr_type parse_hdr(const char* s, size_t sz)
	{
		const char* rn = "\r\n";
		const char* d = ": ";

		const char* e = s + sz;
		hdr_type hdr;
		for(;;)
		{
			const auto p = std::search(s, e, rn, rn + 2);
			const auto f = std::search(s, p, d, d + 2);
			if(f == p)
			{
				hdr.clear();
				break;
			}
			hdr[std::string(s, f)].push_back(std::string(f + 2, p));
			s = p + 2;
			if(s == e)
				break;
		}
		return hdr;
	}

	hdr_type get_hdr()
	{
		std::string h;
		br->set_pos(p1 + 2);
		br->readN(h, p2 - p1);
		return parse_hdr(h.c_str(), h.length());
	}

	std::vector<uint8_t> get_body()
	{
		br->set_pos(p2 + 4);
		std::vector<uint8_t> res;
		br->readN(res, br->get_rsize());
		return res;
	}
};
