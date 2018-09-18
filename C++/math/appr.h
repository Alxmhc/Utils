//a.x*r.x+a.y*r.y+a.z=0; b.x*r.x+b.y*r.y+b.z=0
vt2 eq1_2(const vt3 &a, const vt3 &b)
{
	vt3 r = vt3::pr(a,b);
	return vt2(r.x/r.z, r.y/r.z);
}

//y = a + bx
vt2 appr_lin(const std::vector<vt2> &v)
{
	auto n = v.size();
	vt3 a,b;
	a.x = static_cast<long double>(n);
	while(n--)
	{
		a.y += v[n].x;
		b.y += v[n].x * v[n].x;
		a.z -= v[n].y;
		b.z -= v[n].x * v[n].y;
	}
	b.x = a.y;
	return eq1_2(a, b);
}

//y = a + b/x
vt2 appr_hyp(const std::vector<vt2> &v)
{
	auto n = v.size();
	vt3 a,b;
	a.x = static_cast<long double>(n);
	while(n--)
	{
		long double k = 1.0/v[n].x;
		a.y += k;
		b.y += k * k;
		a.z -= v[n].y;
		b.z -= k * v[n].y;
	}
	b.x = a.y;
	return eq1_2(a, b);
}
