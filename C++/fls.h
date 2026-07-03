#ifndef H_FLS
#define H_FLS

#include <filesystem>

namespace fl_s
{
	template<typename C>
	bool create_dir(const C* pth)
	{
		return std::filesystem::create_directory(pth);
	}

	template<typename C>
	bool create_dirs(const C* pth)
	{
		return std::filesystem::create_directories(pth);
	}

	template<typename C>
	bool copy(const C* p1, const C* p2)
	{
		const std::filesystem::path npth(p2);
		std::filesystem::create_directories(npth.parent_path());
		try
		{
			std::filesystem::copy(p1, npth, std::filesystem::copy_options::recursive);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	template<typename C>
	bool move(const C* p1, const C* p2)
	{
		const std::filesystem::path npth(p2);
		std::filesystem::create_directories(npth.parent_path());
		try
		{
			std::filesystem::rename(p1, npth);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

	template<typename C>
	bool del(const C* pth)
	{
		return std::filesystem::remove(pth);
	}

	template<typename C>
	void del_dirs(const C* pth)
	{
		std::filesystem::remove_all(pth);
	}

	template<typename C>
	std::basic_string<C> to_string(const std::filesystem::path&);
	template<>
	std::basic_string<char> to_string(const std::filesystem::path &p)
	{
		return p.generic_string();
	}
	template<>
	std::basic_string<wchar_t> to_string(const std::filesystem::path &p)
	{
		return p.generic_wstring();
	}

	template<typename C, class T>
	void proc_dir(const C* p, T &st)
	{
		for (const auto &e : std::filesystem::recursive_directory_iterator(p))
		{
			auto name = to_string<C>(e.path());
			if (e.is_directory())
			{
				name.push_back('/');
			}
			st(name);
		}
	}
}

#endif
