#ifndef H_CR
#define H_CR

#include "../../base/math_.h"
#include "../vt2.h"

namespace geom
{
	class p_cr
	{
		long double r;
	public:
		p_cr(long double c) : r(c) {}

		static long double sq(long double r)
		{
			return pi*r*r;
		}

		long double sq() const
		{
			return sq(r);
		}
	};

	class cr : public p_cr
	{
		vt2 cnt;
	public:
		cr(const vt2 &c, long double r) : p_cr(r), cnt(c) {}
	};
}

#endif
