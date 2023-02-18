#ifndef H_FNC
#define H_FNC

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
		dn += dn;
		for(unsigned long int i=1; i<n; i++)
		{
			a += d+d;
			s += dn*(f(a)+2*f(a+d));
		}
		return s;
	}
}

#endif
