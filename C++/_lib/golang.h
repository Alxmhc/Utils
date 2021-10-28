typedef int8_t  GoInt8;
typedef int16_t GoInt16;
typedef int32_t GoInt32;
typedef int64_t GoInt64;

typedef uint8_t  GoUint8;
typedef uint16_t GoUint16;
typedef uint32_t GoUint32;
typedef uint64_t GoUint64;

typedef GoInt64  GoInt;
typedef GoUint64 GoUint;

class GoSlice
{
	void *data;
	GoInt len;
	GoInt cap;
public:
	GoSlice(uint8_t *v, size_t n) : data(v), len(n), cap(n) {}
};

class GoString
{
	const char* p;
	const ptrdiff_t n;
public:
	GoString(const char* s) : p(s), n(strlen(s)) {}
	GoString(const char* s, size_t k) : p(s), n(k) {}
	GoString(const std::string &s) : p(s.c_str()), n(s.length()) {}

	std::string string() const
	{
		return std::string(p, n);
	}
};
