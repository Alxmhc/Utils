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
}

#endif
