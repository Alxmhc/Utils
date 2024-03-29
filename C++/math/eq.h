#ifndef H_EQ
#define H_EQ

#include "v2/vt2.h"
#include "v3/vt3.h"

//a.x*r.x+a.y*r.y+a.z=0; b.x*r.x+b.y*r.y+b.z=0
vt2 eq1_2(const vt3 &a, const vt3 &b)
{
	vt3 r = vt3::pr(a,b);
	return vt2(r.x/r.z, r.y/r.z);
}

#endif
