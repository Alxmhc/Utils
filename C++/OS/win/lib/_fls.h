BOOL MoveFileEx_(LPCSTR old_path, LPCSTR new_path, DWORD flags)
{
	return MoveFileExA(old_path, new_path, flags);
}
BOOL MoveFileEx_(LPCWSTR old_path, LPCWSTR new_path, DWORD flags)
{
	return MoveFileExW(old_path, new_path, flags);
}
