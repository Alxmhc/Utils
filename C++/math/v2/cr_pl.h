#ifndef H_CR_PL
#define H_CR_PL

class cr_pl
{
public:
	long double r, f;
	cr_pl() : r(0.0), f(0.0) {}
	cr_pl(long double s, long double a) : r(s), f(a) {}
	cr_pl(const vt2 &v) : r(abs(v)), f(atan2(v.y, v.x)) {}

	vt2 v() const
	{
		return vt2(r*cos(f), r*sin(f));
	}

	const cr_pl& operator+=(const cr_pl &t)
	{
		vt2 a = v();
		a += t.v();
		*this = a;
		return *this;
	}
};

long double abs(const cr_pl &c)
{
	return c.r;
}

#endif
