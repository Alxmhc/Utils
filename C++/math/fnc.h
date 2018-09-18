namespace fnc
{
	template<class F>
	long double dif(F f, long double x, long double d)
	{
		return (f(x+d)-f(x-d))/(d+d);
	}

	template<class F>
	long double dif2(F f,long double x,long double d)
	{
		return (8*(f(x+d)-f(x-d))-(f(x+d+d)-f(x-d-d)))/(12*d);
	}

	template<class F>
	long double intg_Sm(F f, long double a, long double b, long double d)
	{
		auto n = static_cast<unsigned long int>(ceil((b-a)/(d+d)));
		d = (b-a)/(n+n);
		long double dn = d/3.0;
		long double s = dn*(f(a)+f(b)+4*f(a+d));
		long double x = a+d+d;
		dn += dn;
		for(unsigned long int i=1; i<n; i++)
		{
			s += dn*(f(x)+2*f(x+d));
			x += d+d;
		}
		return s;
	}

	template<class F>
	long double solve_eq(F f, long double a, long double b, long double d)
	{
		if(f(a) > 0){auto t=a; a=b; b=t;}
		for(;;)
		{
			long double x = 0.5*(a+b);
			long double y = f(x);
			if(y < 0)
			{
				if(y > -d)
					return x;
				a = x;
			}
			else
			{
				if(y < d)
					return x;
				b = x;
			}
		}
	}
}
