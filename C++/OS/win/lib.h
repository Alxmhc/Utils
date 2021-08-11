class dlib
{
	HINSTANCE inst;
public:
	dlib() : inst(nullptr) {}
	~dlib()
	{
		free();
	}

	void load(const char *pth)
	{
		inst = LoadLibrary_(pth);
	}
	void free()
	{
		if(inst != nullptr)
		{
			FreeLibrary(inst);
			inst = nullptr;
		}
	}

	void* get_addr(const char *fname) const
	{
		if(inst == nullptr)
			return nullptr;
		return GetProcAddress(inst, fname);
	}
};
