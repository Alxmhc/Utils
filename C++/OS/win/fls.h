namespace fl_s
{
	bool create_dir(const char* pth)
	{
		return CreateDirectoryA(pth, NULL) == TRUE;
	}
	bool create_dir(const wchar_t* pth)
	{
		return CreateDirectoryW(pth, NULL) == TRUE;
	}

	bool del_file(const char* filename)
	{
		return DeleteFileA(filename) == TRUE;
	}
	bool del_file(const wchar_t* filename)
	{
		return DeleteFileW(filename) == TRUE;
	}

	bool del_dir(const char* dirname)
	{
		return RemoveDirectoryA(dirname) == TRUE;
	}
	bool del_dir(const wchar_t* dirname)
	{
		return RemoveDirectoryW(dirname) == TRUE;
	}

	class list
	{
		template<typename S> struct WIN32_FIND_DATA_{};
		template<> struct WIN32_FIND_DATA_<char>{typedef WIN32_FIND_DATAA T;};
		template<> struct WIN32_FIND_DATA_<std::string>{typedef WIN32_FIND_DATAA T;};
		template<> struct WIN32_FIND_DATA_<wchar_t>{typedef WIN32_FIND_DATAW T;};
		template<> struct WIN32_FIND_DATA_<std::wstring>{typedef WIN32_FIND_DATAW T;};

		static HANDLE FindFirstFile_(LPCSTR filename, LPWIN32_FIND_DATAA ffd)
		{
			return FindFirstFileA(filename, ffd);
		}
		static HANDLE FindFirstFile_(LPCWSTR filename, LPWIN32_FIND_DATAW ffd)
		{
			return FindFirstFileW(filename, ffd);
		}
		static bool FindNextFile_(HANDLE hff, LPWIN32_FIND_DATAA ffd)
		{
			return FindNextFileA(hff, ffd) == TRUE;
		}
		static bool FindNextFile_(HANDLE hff, LPWIN32_FIND_DATAW ffd)
		{
			return FindNextFileW(hff, ffd) == TRUE;
		}

		template<typename S>
		static void dir_ex(const S &d, std::vector<S> &r, bool (*filter)(const S&), int depth, bool is_dir)
		{
			S pth = d;
			pth.push_back('*');
			typename WIN32_FIND_DATA_<S>::T ffd;
			HANDLE hf = FindFirstFile_(pth.c_str(), &ffd);
			if (hf == INVALID_HANDLE_VALUE)
				return;
			do {
				if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if( excl_dir(ffd.cFileName) )
						continue;
					pth = d + ffd.cFileName;
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
				else if(!is_dir)
				{
					pth = d + ffd.cFileName;
					if(filter(pth))
					{
						r.push_back(pth);
					}
				}
			} while ( FindNextFile_(hf, &ffd) );
			FindClose(hf);
		}
	public:
		template<typename C>
		static bool excl_dir(const C* name)
		{
			if(name[0] != '.')
				return false;
			if(name[1] == 0)
				return true;
			return (name[2] == 0 && name[1] == '.');
		}

		template<typename C>
		static std::vector<std::basic_string<C>> dir_files(const C* d, bool (*filter)(const std::basic_string<C>&), int depth = -1)
		{
			std::basic_string<C> p(d);
			p.push_back('/');
			std::vector<std::basic_string<C>> r;
			dir_ex(p, r, filter, depth, false);
			return r;
		}

		template<typename C>
		static std::vector<std::basic_string<C>> dir_folders(const C* d, bool (*filter)(const std::basic_string<C>&), int depth = -1)
		{
			std::basic_string<C> p(d);
			p.push_back('/');
			std::vector<std::basic_string<C>> r;
			dir_ex(p, r, filter, depth, true);
			return r;
		}
	};

	template<typename C>
	bool create_dirs(const C* pth)
	{
		std::basic_string<C> t(pth);
		for(size_t i = 0; i < t.length(); i++)
		{
			if(t[i] == '/' || t[i] == '\\')
			{
				t[i] = 0;
				create_dir(t.c_str());
				t[i] = '/';
			}
		}
		return create_dir(pth);
	}
}
