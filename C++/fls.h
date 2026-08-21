#ifndef H_FLS
#define H_FLS

#include <filesystem>

namespace fl_s
{
	bool create_dir(const std::filesystem::path &pth)
	{
		std::error_code err;
		std::filesystem::create_directory(pth, err);
		return !err;
	}

	bool create_dirs(const std::filesystem::path &pth)
	{
		std::error_code err;
		std::filesystem::create_directories(pth, err);
		return !err;
	}

	bool copy_file(const std::filesystem::path &p1, const std::filesystem::path &p2)
	{
		if (!create_dirs(p2.parent_path()))
			return false;
		std::error_code err;
		std::filesystem::copy_file(p1, p2, err);
		return !err;
	}

	bool copy_dir(const std::filesystem::path &p1, const std::filesystem::path &p2)
	{
		if (!create_dirs(p2.parent_path()))
			return false;
		std::error_code err;
		std::filesystem::copy(p1, p2, std::filesystem::copy_options::recursive, err);
		return !err;
	}

	bool move(const std::filesystem::path &p1, const std::filesystem::path &p2)
	{
		if (!create_dirs(p2.parent_path()))
			return false;
		std::error_code err;
		std::filesystem::rename(p1, p2, err);
		return !err;
	}

	bool del(const std::filesystem::path &pth)
	{
		std::error_code err;
		std::filesystem::remove(pth, err);
		return !err;
	}

	bool del_dirs(const std::filesystem::path &pth)
	{
		std::error_code err;
		std::filesystem::remove_all(pth, err);
		return !err;
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
