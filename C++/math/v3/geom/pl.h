//n*r-p=0; |n|=1; p>=0
class pl
{
	vt3 n;
	long double p;

public:
	pl(const vt3 &a, const vt3 &b, const vt3 &c)
	{
		n = vt3::pr(b-a, c-a);
		n /= abs(n);
		p = dot(a, n);
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

private:
	long double dst_(const vt3 &k) const
	{
		return dot(n, k) - p;
	}
};
