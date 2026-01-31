#ifndef H_VT3
#define H_VT3

class vt3
{
public:
	long double x, y, z;
	vt3() : x(0), y(0), z(0) {}
	vt3(long double a, long double b, long double c) : x(a), y(b), z(c) {}
	vt3(const vt3 &v) : x(v.x), y(v.y), z(v.z) {}

	vt3 operator-() const
	{
		return vt3(-x, -y, -z);
	}

	const vt3& operator+=(const vt3 &a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	const vt3& operator-=(const vt3 &a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	const vt3& operator*=(const vt3 &a)
	{
		x *= a.x;
		y *= a.y;
		z *= a.z;
		return *this;
	}

	vt3 operator-(const vt3 &c) const
	{
		return vt3(x-c.x, y-c.y, z-c.z);
	}

	const vt3& operator*=(long double k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}
	const vt3& operator/=(long double k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}
	vt3 operator*(long double k) const
	{
		return vt3(k*x, k*y, k*z);
	}
	vt3 operator/(long double k) const
	{
		return vt3(x/k, y/k, z/k);
	}

	static vt3 pr(const vt3 &a, const vt3 &b)
	{
		const auto x = a.y*b.z - a.z*b.y;
		const auto y = a.z*b.x - a.x*b.z;
		const auto z = a.x*b.y - a.y*b.x;
		return vt3(x, y, z);
	}
};

long double dot(const vt3 &a, const vt3 &b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

#endif
