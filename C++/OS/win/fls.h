class fl_s
{
	static bool is_n(const std::string &s){return s == "." || s == "..";}
	static bool is_n(const std::wstring &s){return s == L"." || s == L"..";}

	template<typename S>
	static void dir_ex(const S &d, std::vector<S> &r, bool (*filter)(const S&), int depth, bool is_dir)
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
					dir_ex(pth, r, filter, depth - 1, is_dir);
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
		std::basic_string<C> p(d);
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		std::vector<std::basic_string<C>> r;
		dir_ex(p, r, filter, depth, false);
		return r;
	}

	template<typename C>
	static std::vector<std::basic_string<C>> dir_folders(const C* d, bool (*filter)(const std::basic_string<C>&), int depth = -1)
	{
		std::basic_string<C> p(d);
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		std::vector<std::basic_string<C>> r;
		dir_ex(p, r, filter, depth, true);
		return r;
	}
};

bool create_dir(const char* pth)
{
	return CreateDirectoryA(pth, NULL) == TRUE;
}
bool create_dir(const wchar_t* pth)
{
	return CreateDirectoryW(pth, NULL) == TRUE;
}

template<typename C>
bool create_dirs(const C* pth)
{
	std::basic_string<C> t(pth);
	for(size_t i = 0; i < t.length(); i++)
	{
		if(t[i] =='/' || t[i] == '\\')
		{
			t[i] = 0;
			create_dir(t.c_str());
			t[i] = '/';
		}
	}
	return create_dir(pth);
}
