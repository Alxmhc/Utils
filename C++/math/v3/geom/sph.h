#ifndef H_SPH
#define H_SPH

#include "../../base/math_.h"
#include "../vt3.h"

namespace geom
{
	class p_sph
	{
		long double r;
	public:
		p_sph(long double c) : r(c) {}

		static long double vl(long double r)
		{
			return 4.0*(pi/3.0)*r*r*r;
		}

		long double vl() const
		{
			return vl(r);
		}

		//volume cut off by a plane at a distance d from the center
		long double vl_p(long double d) const
		{
			if(d<r)
				return (pi/3.0)*(r*r*(2*r-3*d)+d*d*d);
			return 0.0;
		}
	};

	class sph : public p_sph
	{
		vt3 cnt;
	public:
		sph(const vt3 &c, long double r) : p_sph(r), cnt(c) {}
	};
}

#endif
