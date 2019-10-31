class cr_pl
{
public:
	long double r, f;
	cr_pl(void) : r(0.0), f(0.0) {}
	cr_pl(long double s, long double a) : r(s), f(a) {}
};

long double abs(const cr_pl &c)
{
	return c.r;
}

vt2 to_dcr(const cr_pl &c)
{
	return vt2(c.r*cos(c.f), c.r*sin(c.f));
}

cr_pl to_pcr(const vt2 &c)
{
	return cr_pl(abs(c), atan2(c.y, c.x));
}
