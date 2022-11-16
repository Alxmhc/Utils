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

	hdr_type get_hdr()
	{
		br->set_pos(p1 + 2);
		std::string h;
		br->readN(h, p2 - p1);
		hdr_type hdr;
		size_t p = 0;
		for(;;)
		{
			const auto p1 = h.find("\r\n", p);
			std::string tmp = h.substr(p, p1 - p);
			p = tmp.find(": ");
			if(p == std::string::npos)
			{
				hdr.clear();
				break;
			}
			hdr[tmp.substr(0, p)].push_back( tmp.substr(p+2) );
			p = p1 + 2;
			if(p == h.length())
				break;
		}
		return hdr;
	}

	std::vector<uint8_t> get_body()
	{
		br->set_pos(p2 + 4);
		std::vector<uint8_t> res;
		br->readN(res, br->get_rsize());
		return res;
	}
};
