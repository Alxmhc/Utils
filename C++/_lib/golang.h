typedef int8_t  GoInt8;
typedef int16_t GoInt16;
typedef int32_t GoInt32;
typedef int64_t GoInt64;

typedef uint8_t  GoUint8;
typedef uint16_t GoUint16;
typedef uint32_t GoUint32;
typedef uint64_t GoUint64;

class GoString
{
	const char* p;
	ptrdiff_t n;
public:
	GoString(const char* s) : p(s), n(strlen(s)) {}
	GoString(const char* s, size_t k) : p(s), n(k) {}
	GoString(const std::string &s) : p(s.c_str()), n(s.length()) {}

	std::string string() const
	{
		return std::string(p, n);
	}
};
