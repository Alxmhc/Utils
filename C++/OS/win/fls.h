#ifndef H_FLS
#define H_FLS

#include <vector>
#include <string>

#include <windows.h>

namespace fl_s
{
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

	template<typename S>
	class dInf
	{
		HANDLE hf;
		WIN32_FIND_DATA_<S> ffd;
		bool isFin;

		void Close()
		{
			if(hf != nullptr)
			{
				FindClose(hf);
				hf = nullptr;
			}
		}
	public:
		static bool is_pass(const S &name)
		{
			if(name.length() == 2)
				return name[0] == '.' && name[1] == '/';
			if(name.length() == 3)
				return name[0] == '.' && name[1] == '.' && name[2] == '/';
			return false;
		}

		dInf() : hf(nullptr), isFin(true) {}

		~dInf()
		{
			Close();
		}

		void Init(S pth)
		{
			Close();
			pth.push_back('*');
			hf = ffd.FirstFile(pth.c_str());
			isFin = (hf == INVALID_HANDLE_VALUE);
		}

		bool nxt(S &res)
		{
			if(isFin)
				return false;
			res = ffd.fd.cFileName;
			if(ffd.fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				res.push_back('/');
			}
			isFin = !ffd.NextFile(hf);
			return true;
		}
	};


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
				p[i] = pth[i];
			}
		}
		return create_dir(pth);
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

	template<typename C, class T>
	void proc_dir(const C* p, T &st, int depth = -1)
	{
		typedef std::basic_string<C> S;

		S pth(p);
		if(pth.back() != '/')
		{
			pth.push_back('/');
		}

		std::vector<std::pair<S, int>> pths;
		pths.push_back(std::pair<S, int>(pth, depth));
		dInf<S> g;
		while(!pths.empty())
		{
			const auto p = std::move(pths.back());
			pths.pop_back();
			g.Init(p.first);

			S name;
			while(g.nxt(name))
			{
				if(g.is_pass(name))
					continue;
				name = p.first + name;
				if(name.back() == '/' && p.second != 0)
				{
					pths.push_back(std::pair<S, int>(name, p.second - 1));
				}
				st(name);
			}
		}
	}

	template<typename C, class T>
	std::vector<std::basic_string<C>> list_dir(const C* pth, T &fltr, int depth = -1)
	{
		typedef std::basic_string<C> S;

		std::vector<S> res;
		auto fnc = [&](const S &s){if(fltr(s)){res.push_back(s);}};
		proc_dir(pth, fnc, depth);
		return res;
	}

	template<typename C>
	void del_dirs(const C* p)
	{
		typedef std::basic_string<C> S;

		S pth(p);
		if(pth.back() != '/')
		{
			pth.push_back('/');
		}

		std::vector<S> pths;
		pths.push_back(pth);
		std::vector<S> d(pths);
		dInf<S> g;
		while(!pths.empty())
		{
			const auto p = std::move(pths.back());
			pths.pop_back();
			g.Init(p);

			S name;
			while(g.nxt(name))
			{
				if(g.is_pass(name))
					continue;
				name = p + name;
				if(name.back() != '/')
				{
					del_file(name.c_str());
					continue;
				}
				pths.push_back(name);
				d.push_back(name);
			}
		}
		while(!d.empty())
		{
			del_dir(d.back().c_str());
			d.pop_back();
		}
	}
}

#endif
