#ifndef H_DLIB
#define H_DLIB

#include <windows.h>

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
		inst = LoadLibraryA(pth);
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
