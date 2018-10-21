class vt2{
public:
	long double x, y;
	vt2(void){x=0.0; y=0.0;}
	vt2(long double a, long double b){x=a; y=b;}
	vt2(const vt2 &v){x=v.x; y=v.y;}

	vt2 operator-() const
	{
		return vt2(-x, -y);
	}

	vt2 operator+(const vt2 &a) const
	{
		return vt2(x+a.x, y+a.y);
	}

	vt2 operator-(const vt2 &a) const
	{
		return vt2(x-a.x, y-a.y);
	}

	vt2 operator*(long double k) const
	{
		return vt2(x*k, y*k);
	}

	const vt2& operator+=(const vt2 &a)
	{
		x+=a.x; y+=a.y;
		return *this;
	}

	const vt2& operator-=(const vt2 &a)
	{
		x-=a.x; y-=a.y;
		return *this;
	}

	const vt2& operator*=(long double k)
	{
		x*=k; y*=k;
		return *this;
	}

	long double operator*(const vt2 &a) const
	{
		return x*a.x + y*a.y;
	}
};
