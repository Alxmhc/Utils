#ifndef H_FLS
#define H_FLS

#include <filesystem>

namespace fl_s
{
	template<typename C>
	bool del(const C* pth)
	{
		return std::filesystem::remove(pth);
	}

	template<typename C>
	std::basic_string<C> to_string(const std::filesystem::path& p);
	template<>
	std::basic_string<char> to_string(const std::filesystem::path& p)
	{
		return p.string();
	}
	template<>
	std::basic_string<wchar_t> to_string(const std::filesystem::path& p)
	{
		return p.wstring();
	}

	template<typename C, class T>
	void proc_dir(const C* p, T &st)
	{
		for (const auto &e : std::filesystem::recursive_directory_iterator(p))
		{
			auto name = to_string<C>(e.path());
			std::replace(name.begin(), name.end(), '\\', '/');
			if (e.is_directory())
			{
				name.push_back('/');
			}
			st(name);
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
}

#endif
