namespace fl_pr
{
	namespace F_pem
	{
		std::map<std::string, std::vector<uint8_t>> read(byteReader &s)
		{
			std::map<std::string, std::vector<uint8_t>> res;

			std::vector<uint8_t> vt;
			bw_array bw(vt);
			convert::base64::Decoder d(convert::base64::dct_std, bw);
			for(;;)
			{
				auto st = s.read_string('\n');
				if(st.length() == 0)
					break;
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
					st = st.substr(9, st.length() - 14);
					d.Final();
					res.insert(std::pair<std::string, std::vector<uint8_t>>(st, vt));
					vt.clear();
				}
			}
			return res;
		}
	}
}
