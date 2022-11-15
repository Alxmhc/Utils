struct HTTP
{
	std::string fl;
	std::map<std::string, std::vector<std::string>> hdr;
	std::vector<uint8_t> body;

	bool parse(byteReader &br)
	{
		if( !br.read_string(bytes("\r\n"), 2, fl) )
			return false;
		for(;;)
		{
			std::string tmp;
			if( !br.read_string(bytes("\r\n"), 2, tmp) )
				return false;
			if(tmp.empty())
				break;
			const auto p = tmp.find(": ");
			if(p == std::string::npos)
				return false;
			std::string h = tmp.substr(0, p);
			std::transform(h.begin(), h.end(), h.begin(), ::tolower);
			hdr[h].push_back( tmp.substr(p+2) );
		}
		br.readN(body, br.get_rsize());
		return true;
	}
};
