template<unsigned char HSIZE>
class HASH
{
public:
	static const uint_fast8_t hash_size = HSIZE;

	virtual void Init() = 0;
	virtual void Update(const uint8_t*, const size_t) = 0;
	virtual void Final(uint8_t*) = 0;
};
