const uint8_t* bytes(const std::string &s)
{
	return reinterpret_cast<const uint8_t*>(s.c_str());
}
