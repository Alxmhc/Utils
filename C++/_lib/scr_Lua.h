class scr_Lua
{
	lua_State *s;
public:
	scr_Lua(){s = luaL_newstate();}
	~scr_Lua(){lua_close(s);}

	bool exec(const std::string& fl)
	{
		int r = luaL_loadfile(s, fl.c_str());
		if ( r != LUA_OK )
			return false;
		r = lua_pcall(s, 0, LUA_MULTRET, 0);
		if ( r != LUA_OK )
			return false;
		return true;
	}

	template<typename T>
	void set(const T& val, const char* name)
	{
		set(val);
		lua_setglobal(s, name);
	}

	void get(lua_Integer& val, int ind)
	{
		val = lua_tointeger(s, ind);
	}
	void get(lua_Number& val, int ind)
	{
		val = lua_tonumber(s, ind);
	}
	void get(std::string& val, int ind)
	{
		val = lua_tostring(s, ind);
	}
private:
	void set(lua_Integer val)
	{
		lua_pushinteger(s, val);
	}
	void set(lua_Number val)
	{
		lua_pushnumber(s, val);
	}
	void set(const std::string& val)
	{
		lua_pushstring(s, val.c_str());
	}
};
