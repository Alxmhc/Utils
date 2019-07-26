template<typename C>
bool is_b(const std::basic_string<C> &s, const std::basic_string<C> &f)
{
	auto l = f.length();
	if(l > s.length())
		return false;
	return s.compare(0, l, f) == 0;
}

template<typename C>
bool is_e(const std::basic_string<C> &s, const std::basic_string<C> &f)
{
	auto l = f.length();
	if(l > s.length())
		return false;
	return s.compare(s.length() - l, l, f) == 0;
}

template<typename C>
bool is_b(const std::basic_string<C> &s, const C *f){return is_b(s, std::basic_string<C>(f));}
template<typename C>
bool is_e(const std::basic_string<C> &s, const C *f){return is_e(s, std::basic_string<C>(f));}

std::string read_string(std::istream &s)
{
	std::string r;
	for(;;)
	{
		char c = s.get();
		if(c == 0)
			break;
		r += c;
	}
	return r;
}
