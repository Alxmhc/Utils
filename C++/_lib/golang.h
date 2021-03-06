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
