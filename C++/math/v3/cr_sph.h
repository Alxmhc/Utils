#ifndef H_CR_SPH
#define H_CR_SPH

#include "../base/math_.h"
#include "vt3.h"

class cr_sph
{
public:
	long double r, t, f;
	cr_sph() : r(0.0), t(0.0), f(0.0) {}
	cr_sph(long double s, long double a, long double b) : r(s), t(a), f(b) {}
	cr_sph(const vt3 &v) : r(vt::abs(v)), t(std::acos(v.z/r)), f(std::atan2(v.y, v.x)) {}

	vt3 v() const
	{
		const long double a = r * std::sin(t);
		return vt3(a*std::cos(f), a*std::sin(f), r*std::cos(t));
	}
};

#endif
