class cr_pl
{
public:
	long double r, f;
	cr_pl() : r(0.0), f(0.0) {}
	cr_pl(long double s, long double a) : r(s), f(a) {}
	cr_pl(const vt2 &v) : r(abs(v)), f(atan2(v.y, v.x)) {}

	operator vt2() const
	{
		return vt2(r*cos(f), r*sin(f));
	}

	cr_pl operator+(const cr_pl &t) const
	{
		vt2 a = vt2(*this);
		vt2 b = vt2(t);
		return cr_pl(a + b);
	}
};

long double abs(const cr_pl &c)
{
	return c.r;
}
