#include "sqlite\sqlite3.h"

class sqlite_e
{
public:
	sqlite_e(const std::string nm){r = sqlite3_open(nm.c_str(), &bd); err = 0;}
	~sqlite_e(){sqlite3_close(bd);}

	void exec(const std::string SQL, sqlite3_callback f=0){
		r = sqlite3_exec(bd, SQL.c_str(), f, 0, &err);
		if(r){sqlite3_free(err);}
	}
private:
	sqlite3 *bd;
	int r;
	char *err;
};
