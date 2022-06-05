class fl_s
{
	static bool is_n(const std::string &s){return s == "." || s == "..";}
	static bool is_n(const std::wstring &s){return s == L"." || s == L"..";}

	template<typename C>
	static std::vector<std::basic_string<C>> dir_ex(const C* d, bool (*filter)(const std::basic_string<C>&), int depth, bool is_dir)
	{
		std::basic_string<C> p(d);
		std::replace(p.begin(), p.end(), '\\', '/');
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		std::vector<std::basic_string<C>> r;
		_dir_ex(p, r, filter, depth, is_dir);
		r.shrink_to_fit();
		return r;
	}

	template<typename S>
	static void _dir_ex(const S &d, std::vector<S> &r, bool (*filter)(const S&), int depth, bool is_dir)
	{
		S n = d;
		n.push_back('*');
		typename WIN32_FIND_DATA_<S>::T ffd;
		HANDLE hf = FindFirstFile_(n.c_str(), &ffd);
		if (hf == INVALID_HANDLE_VALUE)
			return;
		do {
			n = ffd.cFileName;
			S pth = d + n;
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(is_n(n))
					continue;
				if(is_dir && filter(pth))
				{
					r.push_back(pth);
				}
				if(depth != 0)
				{
					pth.push_back('/');
					_dir_ex(pth, r, filter, depth - 1, is_dir);
				}
			}
			else if(!is_dir && filter(pth))
			{
				r.push_back(pth);
			}
		} while ( FindNextFile_(hf, &ffd) );
		FindClose(hf);
	}
public:

	template<typename C>
	static std::vector<std::basic_string<C>> dir_files(const C* d, bool (*filter)(const std::basic_string<C>&), int depth = -1)
	{
		return dir_ex(d, filter, depth, false);
	}

	template<typename C>
	static std::vector<std::basic_string<C>> dir_folders(const C* d, bool (*filter)(const std::basic_string<C>&), int depth = -1)
	{
		return dir_ex(d, filter, depth, true);
	}
};
