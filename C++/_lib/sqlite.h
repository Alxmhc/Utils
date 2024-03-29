class sq3
{
	sqlite3* db;
public:
	sq3() : db(nullptr) {}
	~sq3()
	{
		Close();
	}

	bool Open(const char* fl)
	{
		return sqlite3_open(fl, &db) == 0;
	}
	void Close()
	{
		sqlite3_close(db);
		db = nullptr;
	}

	bool Exec(const char* rq, int (*cb)(void*,int,char**,char**) = nullptr)
	{
		char* err = nullptr;
		auto res = sqlite3_exec(db, rq, cb, nullptr, &err);
		return res == SQLITE_OK;
	}
};
