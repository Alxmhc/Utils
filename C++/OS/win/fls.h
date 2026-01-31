#ifndef H_FLS
#define H_FLS

#include <vector>
#include <string>

#include <windows.h>

DWORD GetFileAttributes_(LPCSTR pth)
{
	return GetFileAttributesA(pth);
}
DWORD GetFileAttributes_(LPCWSTR pth)
{
	return GetFileAttributesW(pth);
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

bool CreateDirectory_(LPCSTR pth, LPSECURITY_ATTRIBUTES attr)
{
	return CreateDirectoryA(pth, attr) == TRUE;
}
bool CreateDirectory_(LPCWSTR pth, LPSECURITY_ATTRIBUTES attr)
{
	return CreateDirectoryW(pth, attr) == TRUE;
}

bool DeleteFile_(LPCSTR pth)
{
	return DeleteFileA(pth) == TRUE;
}
bool DeleteFile_(LPCWSTR pth)
{
	return DeleteFileW(pth) == TRUE;
}

bool RemoveDirectory_(LPCSTR pth)
{
	return RemoveDirectoryA(pth) == TRUE;
}
bool RemoveDirectory_(LPCWSTR pth)
{
	return RemoveDirectoryW(pth) == TRUE;
}

namespace fl_s
{
	template<typename C>
	bool create_dir(const C* pth)
	{
		const auto inf = GetFileAttributes_(pth);
		if(inf == INVALID_FILE_ATTRIBUTES)
			return CreateDirectory_(pth, nullptr) == TRUE;
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

	bool del(const char* pth)
	{
		const auto l = pth[strlen(pth)-1];
		if(l == '/')
			return RemoveDirectory_(pth) == TRUE;
		return DeleteFile_(pth) == TRUE;
	}
	bool del(const wchar_t* pth)
	{
		const auto l = pth[wcslen(pth)-1];
		if(l == '/')
			return RemoveDirectory_(pth) == TRUE;
		return DeleteFile_(pth) == TRUE;
	}

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

	template<typename C, class T>
	void proc_dir(const C* p, T &st)
	{
		typedef std::basic_string<C> S;

		S pth(p);
		std::vector<S> pths;
		pths.push_back(pth);
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
				if(name.back() == '/')
				{
					pths.push_back(name);
				}
				st(name);
			}
		}
	}

	template<typename C, class T>
	std::vector<std::basic_string<C>> list_dir(const C* pth, T &fltr)
	{
		typedef std::basic_string<C> S;

		std::vector<S> res;
		auto fnc = [&](const S &s){if(fltr(s)){res.push_back(s);}};
		proc_dir(pth, fnc);
		return res;
	}

	template<typename C>
	void del_dirs(const C* p)
	{
		typedef std::basic_string<C> S;

		S pth(p);
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
					DeleteFile_(name.c_str());
					continue;
				}
				pths.push_back(name);
				d.push_back(name);
			}
		}
		while(!d.empty())
		{
			RemoveDirectory_(d.back().c_str());
			d.pop_back();
		}
	}
}

#endif
