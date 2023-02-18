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
	public:
		template<typename S>
		static bool del_a(const S &pth)
		{
			if(pth.back() == '/')
				return del_dir(pth.c_str());
			return del_file(pth.c_str());
		}

		template<typename S, class T>
		static void proc_dir(const S &pth, T &st, int depth = -1)
		{
			dInf<S> cl(pth);
			for(;;)
			{
				const auto name = cl.nxt();
				if(name.empty())
					break;
				const auto p = pth + name;
				if(name.back() == '/')
				{
					if(name[0] == '.')
					{
						if(name[1] == '/')
							continue;
						if(name[1] == '.' && name[2] == '/')
							continue;
					}
					if(depth != 0)
					{
						proc_dir(p, st, depth - 1);
					}
				}
				st(p);
			}
		}
	};

	template<typename C, class T>
	static void proc_dir(const C* pth, T &st, int depth = -1)
	{
		std::basic_string<C> p(pth);
		if(p.back() != '/')
		{
			p.push_back('/');
		}
		list::proc_dir(p, st, depth);
	}

	template<typename C>
	bool create_dirs(const C* pth)
	{
		std::basic_string<C> p(pth);
		for(std::size_t i = 0; i < p.length() - 1; i++)
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
		list::proc_dir(p, list::del_a<std::basic_string<C>>);
		del_dir(pth);
	}
}
