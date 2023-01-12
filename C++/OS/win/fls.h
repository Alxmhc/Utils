namespace fl_s
{
	bool create_dir(const char* pth)
	{
		const auto inf = GetFileAttributesA(pth);
		if(inf == INVALID_FILE_ATTRIBUTES)
			return CreateDirectoryA(pth, nullptr) == TRUE;
		return (inf & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}
	bool create_dir(const wchar_t* pth)
	{
		const auto inf = GetFileAttributesW(pth);
		if(inf == INVALID_FILE_ATTRIBUTES)
			return CreateDirectoryW(pth, nullptr) == TRUE;
		return (inf & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}

	bool del_file(const char* pth)
	{
		return DeleteFileA(pth) == TRUE;
	}
	bool del_file(const wchar_t* pth)
	{
		return DeleteFileW(pth) == TRUE;
	}

	bool del_dir(const char* pth)
	{
		return RemoveDirectoryA(pth) == TRUE;
	}
	bool del_dir(const wchar_t* pth)
	{
		return RemoveDirectoryW(pth) == TRUE;
	}

	template<typename S>
	struct WIN32_FIND_DATA_{};

	template<>
	struct WIN32_FIND_DATA_<std::string>
	{
		WIN32_FIND_DATAA fd;
		HANDLE FirstFile(LPCSTR filename)
		{
			return FindFirstFileA(filename, &fd);
		}
		bool NextFile(HANDLE hf)
		{
			return FindNextFileA(hf, &fd) == TRUE;
		}
	};

	template<>
	struct WIN32_FIND_DATA_<std::wstring>
	{
		WIN32_FIND_DATAW fd;
		HANDLE FirstFile(LPCWSTR filename)
		{
			return FindFirstFileW(filename, &fd);
		}
		bool NextFile(HANDLE hf)
		{
			return FindNextFileW(hf, &fd) == TRUE;
		}
	};

	class list
	{
		template<typename S>
		class dInf
		{
			HANDLE hf;
			WIN32_FIND_DATA_<S> ffd;
			bool isFin;
		public:
			dInf(S pth)
			{
				pth.push_back('*');
				hf = ffd.FirstFile(pth.c_str());
				isFin = (hf == INVALID_HANDLE_VALUE);
			}
			~dInf()
			{
				FindClose(hf);
			}

			S nxt()
			{
				S res;
				if(isFin)
					return res;
				res = ffd.fd.cFileName;
				if(ffd.fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					res.push_back('/');
				}
				isFin = !ffd.NextFile(hf);
				return res;
			}
		};

		template<typename S>
		static bool excl_dir(const S &name)
		{
			if(name[0] != '.')
				return false;
			if(name[1] == '/')
				return true;
			return (name[2] == '/' && name[1] == '.');
		}

		template<typename S>
		static void dir_ex(const S &d, std::vector<S> &r, bool (*filter)(const S&), int depth, bool is_dir)
		{
			dInf<S> cl(d);
			for(;;)
			{
				const auto name = cl.nxt();
				if(name.empty())
					break;
				if(name.back() == '/')
				{
					if( excl_dir(name) )
						continue;
					const auto pth = d + name;
					if(is_dir && filter(pth))
					{
						r.push_back(pth);
					}
					if(depth != 0)
					{
						dir_ex(pth, r, filter, depth - 1, is_dir);
					}
				}
				else if(!is_dir)
				{
					const auto pth = d + name;
					if(filter(pth))
					{
						r.push_back(pth);
					}
				}
			}
		}
	public:
		template<typename S>
		static std::vector<S> dir_files(const S &pth, bool (*filter)(const S&), int depth = -1)
		{
			std::vector<S> r;
			dir_ex(pth, r, filter, depth, false);
			return r;
		}

		template<typename S>
		static std::vector<S> dir_folders(const S &pth, bool (*filter)(const S&), int depth = -1)
		{
			std::vector<S> r;
			dir_ex(pth, r, filter, depth, true);
			return r;
		}

		template<typename S>
		static void del_dirs(const S &pth)
		{
			dInf<S> cl(pth);
			for(;;)
			{
				const auto name = cl.nxt();
				if(name.empty())
					break;
				if(name.back() == '/')
				{
					if( excl_dir(name) )
						continue;
					del_dirs(pth + name);
				}
				else
				{
					del_file((pth + name).c_str());
				}
			}
			del_dir(pth.c_str());
		}
	};

	template<typename C>
	static std::vector<std::basic_string<C>> dir_files(const C* pth, bool (*filter)(const std::basic_string<C>&), int depth = -1)
	{
		std::basic_string<C> p(pth);
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		return list::dir_files(p, filter, depth);
	}

	template<typename C>
	static std::vector<std::basic_string<C>> dir_folders(const C* pth, bool (*filter)(const std::basic_string<C>&), int depth = -1)
	{
		std::basic_string<C> p(pth);
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		return list::dir_folders(p, filter, depth);
	}

	template<typename C>
	bool create_dirs(const C* pth)
	{
		std::basic_string<C> p(pth);
		for(size_t i = 0; i < p.length() - 1; i++)
		{
			if(p[i] == '/' || p[i] == '\\')
			{
				p[i] = 0;
				if( !create_dir(p.c_str()) )
					return false;
				p[i] = '/';
			}
		}
		return create_dir(pth);
	}

	template<typename C>
	void del_dirs(const C* pth)
	{
		std::basic_string<C> p(pth);
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		list::del_dirs(p);
	}
}
