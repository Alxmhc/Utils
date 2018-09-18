template <typename B>
B rotl(B x, unsigned char d)
{
	return (x<<d) | (x>>((sizeof(B)<<3)-d));
}

template <typename B>
B rotr(B x, unsigned char d)
{
	return (x>>d) | (x<<((sizeof(B)<<3)-d));
}
