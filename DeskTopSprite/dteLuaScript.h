//////////////////////////////////////////////////////////////////////////
//LUA脚本帮助类v0.98
//提供快捷向LUA注册C/C++函数，以及从脚本获取对象指针
//注意：任何注册请保证注册名不重复，否则结果未定义
//与脚本的交互，必须在调用执行脚本相关语句后进行
//////////////////////////////////////////////////////////////////////////



#pragma once

inline LuaObject CloneLuaTable(LuaObject o)
{
	return o.Clone();
}

inline float GetMemoryLeaved()
{
	MEMORYSTATUS mem;
	GlobalMemoryStatus(&mem);
	SIZE_T size = mem.dwAvailPhys;
	return size/1024.f/1024.f;
}

class dteLuaScript
{
public:
	dteLuaScript()
	{
		state = LuaState::Create(true);
		RegisterCFunctionToLua(CloneLuaTable,"CloneLuaTable");
		RegisterCFunctionToLua(GetMemoryLeaved,"GetMemoryLeaved");
	}
	~dteLuaScript(){LuaState::Destroy(state);}

	//通过对象指针注册类函数
	//参数1：对象指针
	//参数2：函数指针
	//参数3：在脚本中的名字
	template<class Instance, class Func>
	void RegisterFunctionToLua(Instance instance, Func func, const char* name)
	{
		LuaObject luaObject = state->GetGlobals();
		luaObject.RegisterDirect(name, *instance, func);
	}

	//注册C函数
	//参数1：函数指针
	//参数2：在脚本中的名字
	template<class Func>
	void RegisterCFunctionToLua(Func func, const char* name)
	{
		LuaObject luaObject = state->GetGlobals();
		luaObject.RegisterDirect(name, func);
	}

	//从脚本中获取已经定义有的指针(BoxPointer)
	template<class T>
	T* GetPointer(const char* name)
	{
		LuaObject obj = state->GetGlobal(name);
		if (obj.IsUserData())
		{
			void* p = obj.GetUserData();
			return (T*)*(void**)p;
		}
		return NULL;
	}

	//反注册函数
	void UnRegisterFunction(const char* name)
	{
		LuaObject luaObject = state->GetGlobals();
		luaObject.SetNil(name);
	}

	//从脚本中获取Table
	LuaObject GetTableFromScript(const char * name)
	{
		LuaObject obj = state->GetGlobal(name);
		return obj;
	}

	int GetInt(const char* name)
	{
		LuaObject obj = state->GetGlobal(name);
		return obj.ToInteger();
	}

	float GetFloat(const char* name)
	{
		LuaObject obj = state->GetGlobal(name);
		return obj.GetFloat();
	}

	double GetDouble(const char* name)
	{
		LuaObject obj = state->GetGlobal(name);
		return obj.GetDouble();
	}

	bool GetBoolean(const char* name)
	{
		LuaObject obj = state->GetGlobal(name);
		return obj.GetBoolean();
	}

	std::string GetString(const char* name)
	{
		LuaObject obj = state->GetGlobal(name);
		return obj.ToString();
	}

	void GetPos(const char* name, float* x, float* y)
	{
		LuaObject o = state->GetGlobal(name);
		*x = o[1].GetFloat();
		*y = o[2].GetFloat();
	}

	template<typename Func>
	void Register(Func func,const char* name)
	{
		state->GetGlobals().Register(name, func);
	}

	template<class T>
	static T* GetUserdata(LuaObject& obj)
	{
		if (obj.IsUserData())
		{
			void* p = obj.GetUserData();
			return (T*)*(void**)p;
		}
		return NULL;
	}

	template<class T>
	static T* GetUserdata(LuaStackObject& obj)
	{
		if (obj.IsUserData())
		{
			void* p = obj.GetUserData();
			return (T*)*(void**)p;
		}
		return NULL;
	}

	//获得LUA解释器
	LuaState* GetLuaState(){return state;}

	lua_State* GetCSate(){return GetLuaState()->GetCState();}

	operator LuaState*()							{  return state;  }
	operator const LuaState*() const				{  return state;  }
	operator LuaState*() const						{  return state;  }
	LuaState& operator*()							{  return *state; }
	const LuaState& operator*() const				{  return *state; }
	LuaState* operator->()							{  return state;  }
	const LuaState* operator->() const				{  return state;  }
private:
	dteLuaScript(const dteLuaScript&);
	dteLuaScript&	operator= (const dteLuaScript&);

	LuaState* state;//LUA解释器
	
};
