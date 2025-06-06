#ifndef EH_B_INT
#define EH_B_INT

#include "go.h"

#include "../OS/win/dlib.h"
#include "../math/b_int.h"

class b_int_BE
{
	dlib lib;

	typedef GoInt(*pModInv)(GoSlice a, GoSlice m);
	pModInv ModInv_;
	typedef GoInt(*pPowMod)(GoSlice a, GoSlice b, GoSlice m);
	pPowMod PowMod_;
public:
	bool Init(const char* path)
	{
		if(!lib.Read(path))
			return false;
		ModInv_ = reinterpret_cast<pModInv>(lib.get_func("ModInv"));
		if(ModInv_ == nullptr)
			return false;
		PowMod_ = reinterpret_cast<pPowMod>(lib.get_func("PowMod"));
		if(PowMod_ == nullptr)
			return false;
		return true;
	}

	b_uint ModInv(const b_uint &a, const b_uint &m) const
	{
		auto av = a.toB();
		auto mv = m.toB();
		const auto sz = ModInv_(GoSlice(av.data(), av.size()), GoSlice(mv.data(), mv.size()));
		b_uint r;
		r.fromB(mv.data(), sz);
		return r;
	}

	b_uint PowMod(const b_uint &a, const b_uint &b, const b_uint &m) const
	{
		auto av = a.toB();
		auto bv = b.toB();
		auto mv = m.toB();
		const auto sz = PowMod_(GoSlice(av.data(), av.size()), GoSlice(bv.data(), bv.size()), GoSlice(mv.data(), mv.size()));
		b_uint r;
		r.fromB(mv.data(), sz);
		return r;
	}
};

#endif
