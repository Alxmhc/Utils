#ifndef H_B_INT
#define H_B_INT

#include <vector>

#include "go.h"

#include "../OS/win/dlib.h"

class b_int_BE
{
	dlib lib;

	typedef GoInt(*pMod)(GoSlice a, GoSlice m);
	pMod Mod_;
public:
	bool Init(const char* path)
	{
		if(!lib.Read(path))
			return false;
		Mod_ = reinterpret_cast<pMod>(lib.get_addr("Mod"));
		if(Mod_ == nullptr)
			return false;
		return true;
	}

	std::vector<uint8_t> Mod(const std::vector<uint8_t> &a, const std::vector<uint8_t> &m)
	{
		std::vector<uint8_t> res;
		if(a.size() < m.size())
		{
			res = a;
		}
		else
		{
			res = m;
			const auto sz = Mod_(GoSlice(const_cast<uint8_t*>(a.data()), a.size()), GoSlice(res.data(), res.size()));
			if(sz != 0)
			{
				res.resize(sz);
			}
			else
			{
				res.assign(1, 0);
			}
		}
		return res;
	}
};

#endif
