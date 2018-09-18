//n*r-p=0; |n|=1; p>=0
class pl
{
	vt3 n;
	long double p;

public:
	pl(const vt3 &a, const vt3 &b, const vt3 &c)
	{
		n = vt3::pr(b-a, c-a);
		nrm(n);
		p = a*n;
		if(p < 0)
		{
			n = -n;
			p = -p;
		}
	}

	long double dst(const vt3 &k) const
	{
		return abs(dst_(k));
	}

	vt3 prct(const vt3 &c) const
	{
		long double d = dst_(c);
		return c - n*d;
	}

private:
	long double dst_(const vt3 &k) const
	{
		return n*k - p;
	}
};
