namespace fl_pr
{
	namespace F_pem
	{
		std::map<std::string, std::vector<uint8_t>> read(byteReader &s)
		{
			std::map<std::string, std::vector<uint8_t>> res;
			convert::base64::Decoder d(convert::base64::dct_std);
			std::string v;
			for(;;)
			{
				std::string st = s.read_string('\n');
				if(st.length() == 0)
					break;
				if(is_b<char>(st, "-----"))
				{
					if(is_b<char>(st, "-----END "))
					{
						st = st.substr(9, st.length() - 14);
						auto sz = v.length();
						while(v[sz-1] == '=')
						{
							sz--;
						}
						auto vt = d.Convert(reinterpret_cast<const uint8_t*>(v.c_str()), sz);
						v.clear();
						res.insert(std::pair<std::string, std::vector<uint8_t>>(st, vt));
					}
					continue;
				}
				v += st;
			}
			return res;
		}
	}
}
