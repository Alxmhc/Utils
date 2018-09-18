template<typename T>
T gcd(T a, T b)
{
	if(b > a)
	{
		b %= a;
	}
	while (b != 0)
	{
		a %= b;
		if(a == 0)
			return b;
		b %= a;
	}
	return a;
}
