#ifndef H_CR_SPH
#define H_CR_SPH

#include "vt3.h"

class cr_sph
{
public:
	long double r, t, f;
	cr_sph() : r(0.0), t(0.0), f(0.0) {}
	cr_sph(long double s, long double a, long double b) : r(s), t(a), f(b) {}
	cr_sph(const vt3 &v) : r(abs(v)), t(acos(v.z/r)), f(atan2(v.y, v.x)) {}

	vt3 v() const
	{
		const long double a = r * sin(t);
		return vt3(a*cos(f), a*sin(f), r*cos(t));
	}
};

long double abs(const cr_sph &c)
{
	return c.r;
}

#endif
