class cmpl:public vt2{
public:
	explicit cmpl(long double a = 0.0, long double b = 0.0):vt2(a,b){}
	cmpl(const vt2 &v){x=v.x; y=v.y;}

	const cmpl& operator=(const vt2 &a){x=a.x; y=a.y; return *this;}
	const cmpl& operator=(long double a){x=a; y=0.0; return *this;}

	cmpl operator*(const cmpl &a) const
	{
		return cmpl(x*a.x-y*a.y, y*a.x+x*a.y);
	}

	cmpl operator*(long double k) const
	{
		return cmpl(x*k, y*k);
	}

	const cmpl& operator*=(const cmpl &a)
	{
		long double q=x*a.x-y*a.y;
		y=y*a.x+x*a.y;
		x=q;
		return *this;
	}

	const cmpl& operator*=(long double k)
	{
		x*=k;
		y*=k;
		return *this;
	}

	cmpl operator/(const cmpl &a) const
	{
		long double k=1.0/(a.x*a.x+a.y*a.y);
		return cmpl(k*(x*a.x+y*a.y), k*(y*a.x-x*a.y));
	}

	const cmpl& operator/=(const cmpl &a)
	{
		long double k=1.0/(a.x*a.x+a.y*a.y);
		long double q=k*(x*a.x+y*a.y);
		y=k*(y*a.x-x*a.y);
		x=q;
		return *this;
	}
};

long double abs(const cmpl &a)
{
	return sqrt(a.x*a.x+a.y*a.y);
}

cmpl exp(const cmpl &a)
{
	long double m = exp(a.x);
	return cmpl(m*cos(a.y), m*sin(a.y));
}

cmpl cos(const cmpl &a)
{
	return cmpl(cos(a.x)*cosh(a.y), -sin(a.x)*sinh(a.y));
}

cmpl sin(const cmpl &a)
{
	return cmpl(sin(a.x)*cosh(a.y), cos(a.x)*sinh(a.y));
}

cmpl pow(const cmpl &a, long double n)
{
	auto f = n * atan2(a.y, a.x);
	cmpl r(cos(f), sin(f));
	r *= pow(a.x*a.x+a.y*a.y, 0.5*n);
	return r;
}
