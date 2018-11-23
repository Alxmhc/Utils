std::vector<std::string> split_s(const std::string &str, const std::string &d)
{
	std::vector<std::string> r;
	auto e = str.find(d);
	decltype(e)s = 0;
	while(e != std::string::npos)
	{
		r.push_back(str.substr(s, e-s));
		s = e + d.length();
		e = str.find(d, s);
	}
	r.push_back(str.substr(s));
	r.shrink_to_fit();
	return r;
}
