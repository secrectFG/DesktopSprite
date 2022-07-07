//////////////////////////////////////////////////////////////////////////
//LUA�ű�������v0.98
//�ṩ�����LUAע��C/C++�������Լ��ӽű���ȡ����ָ��
//ע�⣺�κ�ע���뱣֤ע�������ظ���������δ����
//��ű��Ľ����������ڵ���ִ�нű�����������
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

	//ͨ������ָ��ע���ຯ��
	//����1������ָ��
	//����2������ָ��
	//����3���ڽű��е�����
	template<class Instance, class Func>
	void RegisterFunctionToLua(Instance instance, Func func, const char* name)
	{
		LuaObject luaObject = state->GetGlobals();
		luaObject.RegisterDirect(name, *instance, func);
	}

	//ע��C����
	//����1������ָ��
	//����2���ڽű��е�����
	template<class Func>
	void RegisterCFunctionToLua(Func func, const char* name)
	{
		LuaObject luaObject = state->GetGlobals();
		luaObject.RegisterDirect(name, func);
	}

	//�ӽű��л�ȡ�Ѿ������е�ָ��(BoxPointer)
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

	//��ע�ắ��
	void UnRegisterFunction(const char* name)
	{
		LuaObject luaObject = state->GetGlobals();
		luaObject.SetNil(name);
	}

	//�ӽű��л�ȡTable
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

	//���LUA������
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

	LuaState* state;//LUA������
	
};
