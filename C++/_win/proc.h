template<typename C>
std::basic_string<C> get_process_path(DWORD PID)
{
	HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
	if(!Handle)
		return std::basic_string<C>();
	C path[MAX_PATH];
	DWORD l = GetModuleFileNameEx_(Handle, 0, path, MAX_PATH);
	CloseHandle(Handle);
	if (l == 0)
		return std::basic_string<C>();
	std::basic_string<C> r(path);
	std::replace(r.begin(), r.end(), '\\', '/');
	return r;
}
