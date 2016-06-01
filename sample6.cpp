// sample3.cpp : LuaTinker �� Ŭ���� ����� �˾ƺ���.
//

extern "C" 
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
};

#include "lua_tinker.h"

// �Լ� ���°� int(*)(lua_State*) �Ǵ� int(*)(lua_State*,T1) �� ��츸 lua_yield() �� �� �� �ִ�.
// �Լ� ���ڰ� �� �ʿ��� ��� lua_tinker.h �� "functor (non-managed)" �ڸ�Ʈ �κ��� �����ؼ� ���� �߰��� ��
int TestFunc(lua_State* L)
{
	printf("# TestFunc ������\n");
	return lua_yield(L, 0);
}

int TestFunc2(lua_State* L, float a)
{
	printf("# TestFunc2(L,%f) ������\n", a);
	return lua_yield(L, 0);
}

int luaL_resume(lua_State *L, lua_State *from, int narg) {
#if LUA_VERSION_NUM >= 503
    return lua_resume(L, from, narg);
#else
    return lua_resume(L, narg);
#endif
}

class TestClass
{
public:

	// �Լ� ���°� int(T::*)(lua_State*) �Ǵ� int(T::*)(lua_State*,T1) �� ��츸 lua_yield() �� �� �� �ִ�.
	// �Լ� ���ڰ� �� �ʿ��� ��� lua_tinker.h �� "class member functor (non-managed)" �ڸ�Ʈ �κ��� �����ؼ� ���� �߰��� ��
	int TestFunc(lua_State* L)
	{
		printf("# TestClass::TestFunc ������\n");
		return lua_yield(L, 0);
	}

	int TestFunc2(lua_State* L, float a)
	{
		printf("# TestClass::TestFunc2(L,%f) ������\n", a);
		return lua_yield(L, 0);
	}
};

int main()
{
	// Lua �� �ʱ�ȭ �Ѵ�.
	lua_State* L = luaL_newstate();

	// Lua �⺻ �Լ����� �ε��Ѵ�.- print() ���
	luaopen_base(L);
	// Lua ���ڿ� �Լ����� �ε��Ѵ�.- string ���
	luaopen_string(L);

	// TestFunc �Լ��� Lua �� ����Ѵ�.
	lua_tinker::def(L, "TestFunc", &TestFunc);
	lua_tinker::def(L, "TestFunc2", &TestFunc2);

	// TestClass Ŭ������ Lua �� �߰��Ѵ�.
	lua_tinker::class_add<TestClass>(L, "TestClass");
	// TestClass �� �Լ��� ����Ѵ�.
	lua_tinker::class_def<TestClass>(L, "TestFunc", &TestClass::TestFunc);
	lua_tinker::class_def<TestClass>(L, "TestFunc2", &TestClass::TestFunc2);

	// TestClass �� ���� ������ �����Ѵ�.
	TestClass g_test;
	lua_tinker::set(L, "g_test", &g_test);

	// sample3.lua ������ �ε��Ѵ�.
	lua_tinker::dofile(L, "sample6.lua");

	// Thread �� �����Ѵ�.
	lua_newthread(L);
	lua_getglobal(L, "ThreadTest");

	// Thread �� �����Ѵ�.
	printf("* luaL_resume() ȣ��\n");
	luaL_resume(L, NULL, 0);

	// Thread �� �ٽ� �����Ѵ�.
	printf("* luaL_resume() ȣ��\n");
	luaL_resume(L, NULL, 0);

	// Thread �� �ٽ� �����Ѵ�.
	printf("* luaL_resume() ȣ��\n");
	luaL_resume(L, NULL, 0);

	// Thread �� �ٽ� �����Ѵ�.
	printf("* luaL_resume() ȣ��\n");
	luaL_resume(L, NULL, 0);

	// Thread �� �ٽ� �����Ѵ�.
	printf("* luaL_resume() ȣ��\n");
	luaL_resume(L, NULL, 0);

	// ���α׷� ����
	lua_close(L);

	return 0;
}

