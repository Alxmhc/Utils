HANDLE CreateMutex_(LPSECURITY_ATTRIBUTES attr, BOOL own, LPCSTR name)
{
	return CreateMutexA(attr, own, name);
}
HANDLE CreateMutex_(LPSECURITY_ATTRIBUTES attr, BOOL own, LPCWSTR name)
{
	return CreateMutexW(attr, own, name);
}
