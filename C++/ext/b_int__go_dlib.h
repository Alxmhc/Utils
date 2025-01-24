#ifndef H_B_INT
#define H_B_INT

#include <vector>

#include "go.h"

#include "../OS/win/dlib.h"

class b_int_BE
{
	dlib lib;

	typedef GoInt(*pMul)(GoSlice a, GoSlice b, GoSlice r);
	pMul Mul_;
	typedef GoInt(*pMod)(GoSlice a, GoSlice m);
	pMod Mod_;
	typedef GoInt(*pModInv)(GoSlice a, GoSlice m);
	pModInv ModInv_;
	typedef GoInt(*pPowMod)(GoSlice a, GoSlice b, GoSlice m);
	pPowMod PowMod_;
public:
	bool Init(const char* path)
	{
		if(!lib.Read(path))
			return false;
		Mul_ = reinterpret_cast<pMul>(lib.get_addr("Mul"));
		if(Mul_ == nullptr)
			return false;
		Mod_ = reinterpret_cast<pMod>(lib.get_addr("Mod"));
		if(Mod_ == nullptr)
			return false;
		ModInv_ = reinterpret_cast<pModInv>(lib.get_addr("ModInv"));
		if(ModInv_ == nullptr)
			return false;
		PowMod_ = reinterpret_cast<pPowMod>(lib.get_addr("PowMod"));
		if(PowMod_ == nullptr)
			return false;
		return true;
	}

	std::vector<uint8_t> Mul(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b) const
	{
		std::vector<uint8_t> res(a.size() + b.size());
		const auto sz = Mul_(GoSlice(const_cast<uint8_t*>(a.data()), a.size()), GoSlice(const_cast<uint8_t*>(b.data()), b.size()), GoSlice(res.data(), res.size()));
		res.resize(sz);
		return res;
	}

	std::vector<uint8_t> Mod(const std::vector<uint8_t> &a, const std::vector<uint8_t> &m) const
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
			res.resize(sz);
		}
		return res;
	}

	std::vector<uint8_t> ModInv(const std::vector<uint8_t> &a, const std::vector<uint8_t> &m) const
	{
		std::vector<uint8_t> res = m;
		const auto sz = ModInv_(GoSlice(const_cast<uint8_t*>(a.data()), a.size()), GoSlice(res.data(), res.size()));
		res.resize(sz);
		return res;
	}

	std::vector<uint8_t> PowMod(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b, const std::vector<uint8_t> &m) const
	{
		std::vector<uint8_t> res = m;
		const auto sz = PowMod_(GoSlice(const_cast<uint8_t*>(a.data()), a.size()), GoSlice(const_cast<uint8_t*>(b.data()), b.size()), GoSlice(res.data(), res.size()));
		res.resize(sz);
		return res;
	}
};

#endif
