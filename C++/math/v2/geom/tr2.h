namespace tr2
{
	long double p_s(long double a, long double b, long double c)
	{
		long double p = 0.5*(a+b+c);
		return sqrt(p*(p-a)*(p-b)*(p-c));
	}

	long double p_r_in(long double a, long double b, long double c)
	{
		long double p = 0.5*(a+b+c);
		return sqrt((p-a)*(p-b)*(p-c)/p);
	}

	long double p_r_out(long double a, long double b, long double c)
	{
		long double p = 0.5*(a+b+c);
		return 0.25*a*b*c/sqrt(p*(p-a)*(p-b)*(p-c));
	}
}
