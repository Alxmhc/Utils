#include <windows.h>

class dlib
{
	HINSTANCE inst;
public:
	dlib() : inst(nullptr) {}
	~dlib()
	{
		free();
	}

	bool load(const char* pth)
	{
		free();
		inst = LoadLibraryA(pth);
		return inst != nullptr;
	}
	void free()
	{
		if(inst != nullptr)
		{
			FreeLibrary(inst);
			inst = nullptr;
		}
	}

	void* get_addr(const char* fname) const
	{
		return GetProcAddress(inst, fname);
	}
};
