#ifndef H_VT2
#define H_VT2

class vt2
{
public:
	long double x, y;
	vt2() : x(0), y(0) {}
	vt2(long double a, long double b) : x(a), y(b) {}
	vt2(const vt2 &v) : x(v.x), y(v.y) {}

	vt2 operator-() const
	{
		return vt2(-x, -y);
	}

	const vt2& operator+=(const vt2 &a)
	{
		x += a.x;
		y += a.y;
		return *this;
	}
	const vt2& operator-=(const vt2 &a)
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}
	const vt2& operator*=(long double k)
	{
		x *= k;
		y *= k;
		return *this;
	}
	const vt2& operator*=(const vt2 &k)
	{
		x *= k.x;
		y *= k.y;
		return *this;
	}
};

long double dot(const vt2 &a, const vt2 &b)
{
	return a.x*b.x + a.y*b.y;
}

long double abs(const vt2 &a)
{
	return sqrt(dot(a,a));
}

#endif
