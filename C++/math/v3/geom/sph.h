class p_sph
{
public:
	long double r;
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
