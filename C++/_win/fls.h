class fl_s
{
	static bool is_n(const std::string &s){return s == "." || s == "..";}
	static bool is_n(const std::wstring &s){return s == L"." || s == L"..";}

	template<typename S, typename F>
	static void _dir_ex(const S &d, std::vector<S> &r, F fn, int depth)
	{
		S n = d;
		n.push_back('*');
		WIN32_FIND_DATA_<S>::T ffd;
		HANDLE hf = FindFirstFile_(n.c_str(), &ffd);
		if (hf == INVALID_HANDLE_VALUE)
			return;
		do{
			n = ffd.cFileName;
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(is_n(n) || !depth)
					continue;
				S pth = d + n;
				pth.push_back('/');
				_dir_ex(pth, r, fn, depth - 1);
			}
			else if(fn(n))
			{
				r.push_back(d + n);
			}
		}while (FindNextFile_(hf, &ffd)!=0);
		FindClose(hf);
	}
public:

	template<typename C, typename F>
	static std::vector<std::basic_string<C>> dir_ex(const C* d, F fn, int depth = -1)
	{
		std::basic_string<C> p(d);
		std::replace(p.begin(), p.end(), '\\', '/');
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		std::vector<std::basic_string<C>> r;
		_dir_ex(p, r, fn, depth);
		r.shrink_to_fit();
		return r;
	}
};
