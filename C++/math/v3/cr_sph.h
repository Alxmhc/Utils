class cr_sph
{
public:
	long double r, t, f;
	cr_sph(void) : r(0.0), t(0.0), f(0.0) {}
	cr_sph(long double s, long double a, long double b) : r(s), t(a), f(b) {}
};

long double abs(const cr_sph &c)
{
	return c.r;
}

vt3 to_dcr(const cr_sph &c)
{
	long double a = c.r*sin(c.t);
	return vt3(a*cos(c.f), a*sin(c.f), c.r*cos(c.t));
}

cr_sph to_sph(const vt3 &c)
{
	long double r = abs(c);
	return cr_sph(r, acos(c.z/r), atan2(c.y, c.x));
}
