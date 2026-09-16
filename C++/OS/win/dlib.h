#ifndef H_DLIB
#define H_DLIB

#include <windows.h>

HMODULE LoadLibrary_(LPCSTR pth)
{
	return LoadLibraryA(pth);
}
HMODULE LoadLibrary_(LPCWSTR pth)
{
	return LoadLibraryW(pth);
}

class dlib
{
	HINSTANCE inst;
public:
	dlib() : inst(nullptr) {}
	~dlib()
	{
		Close();
	}

	bool Read(const char* pth)
	{
		Close();
		inst = LoadLibrary_(pth);
		return inst != nullptr;
	}
	void Close()
	{
		if(inst != nullptr)
		{
			FreeLibrary(inst);
			inst = nullptr;
		}
	}

	void* get_func(const char* fname) const
	{
		return GetProcAddress(inst, fname);
	}
};

#endif
