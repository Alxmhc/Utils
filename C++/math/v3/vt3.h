class vt3{
public:
	long double x, y, z;
	vt3(void) : x(0), y(0), z(0) {}
	vt3(long double a, long double b, long double c) : x(a), y(b), z(c) {}
	vt3(const vt3 &v) : x(v.x), y(v.y), z(v.z) {}

	vt3 operator-() const
	{
		return vt3(-x, -y, -z);
	}

	vt3 operator+(const vt3 &a) const
	{
		return vt3(x+a.x, y+a.y, z+a.z);
	}

	vt3 operator-(const vt3 &a) const
	{
		return vt3(x-a.x, y-a.y, z-a.z);
	}

	vt3 operator*(long double k) const
	{
		return vt3(x*k, y*k, z*k);
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

	const vt3& operator*=(long double k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	long double operator*(const vt3 &a) const
	{
		return x*a.x + y*a.y + z*a.z;
	}

	static vt3 pr(const vt3 &a, const vt3 &b)
	{
		long double x = a.y*b.z - a.z*b.y;
		long double y = a.z*b.x - a.x*b.z;
		long double z = a.x*b.y - a.y*b.x;
		return vt3(x, y, z);
	}
};
