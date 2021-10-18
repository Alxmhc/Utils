namespace fl_pr
{
	namespace F_pem
	{
		std::map<std::string, std::vector<uint8_t>> read(byteReader &s)
		{
			std::map<std::string, std::vector<uint8_t>> res;

			std::vector<uint8_t> vt;
			bw_array bw(vt);
			convert::base64::Decoder d(convert::base64::dct_std);
			d.Set(bw);

			std::string st;
			for(;;)
			{
				s.read_string('\n', st);
				if(st.length() == 0)
					break;
				if(st[st.length()-1] == '\r')
				{
					st.pop_back();
				}
				if(!is_b<char>(st, "-----"))
				{
					auto sz = st.length();
					while(st[sz-1] == '=')
					{
						sz--;
					}
					d.Update(reinterpret_cast<const uint8_t*>(st.c_str()), sz);
				}
				else if(is_b<char>(st, "-----END "))
				{
					st = st.substr(9, st.find('-', 9) - 9);
					d.Final();
					res.insert(std::pair<std::string, std::vector<uint8_t>>(st, vt));
					vt.clear();
				}
			}
			return res;
		}
	}
}
