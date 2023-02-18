#ifndef H_APPR
#define H_APPR

#include <vector>
#include "./eq.h"

//y = a + bx
vt2 appr_lin(const std::vector<vt2> &v)
{
	auto n = v.size();
	vt3 a,b;
	a.x = static_cast<long double>(n);
	while(n--)
	{
		a.y += v[n].x;
		b.y += v[n].x * v[n].x;
		a.z -= v[n].y;
		b.z -= v[n].x * v[n].y;
	}
	b.x = a.y;
	return eq1_2(a, b);
}

//y = a + b/x
vt2 appr_hyp(const std::vector<vt2> &v)
{
	auto n = v.size();
	vt3 a,b;
	a.x = static_cast<long double>(n);
	while(n--)
	{
		long double k = 1.0/v[n].x;
		a.y += k;
		b.y += k * k;
		a.z -= v[n].y;
		b.z -= k * v[n].y;
	}
	b.x = a.y;
	return eq1_2(a, b);
}

#endif
