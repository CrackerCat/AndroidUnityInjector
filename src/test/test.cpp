#include "test.h"

void test_1(lua_State *L) {

    lua_getglobal(L, "print");

    std::string str = std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + __FUNCTION__;
    lua_pushstring(L, str.c_str());

    lua_pushboolean(L, true);

    lua_getglobal(L, "print");
    lua_call(L, 3, 0);
}

void test_2(lua_State *L) {

    luaL_dostring(L, "print('test message')");

    const char *lua_str = R"(
        function sub(a, b)
            return sub_inner(a, b);
        end

        function sub_inner(a, b)
            print(debug.traceback())
            return a + b;
        end
    )";
    luaL_dostring(L, lua_str);

    lua_getglobal(L, "sub");
    lua_pushnumber(L, 10);
    lua_pushnumber(L, 5);
    lua_call(L, 2, 1);
    int result = lua_tonumber(L, -1);
    printf("result: %d\n", result);
    printf("lua_gettop: %p | lua_getstack: %p\n", lua_gettop(L), lua_getstack(L, 0, nullptr));
    lua_pop(L, 1);
    printf("lua_gettop: %p | lua_getstack: %p\n", lua_gettop(L), lua_getstack(L, 0, nullptr));
}

class test_lua {

public:
    test_lua() {
        printf("test_lua ctor\n");
    }

    ~test_lua() {
        printf("~test_lua\n");
    }

    void test() {
        printf("test_lua::test\n");
    }

    std::string m_test_string = "test_lua::m_test_string";
    int m_test_int = 100;
};

void test_3(lua_State *L) {
    printf("test_3\n");
    luabridge::getGlobalNamespace(L)
        .beginClass<test_lua>("test_lua")
        .addConstructor<void (*)()>()
        .addFunction("test", &test_lua::test)
        .addData("m_test_string", &test_lua::m_test_string)
        .addData("m_test_int", &test_lua::m_test_int)
        .endClass();

    static auto test = new test_lua();
    luabridge::setGlobal(L, test, "test_lua");

    luaL_dostring(L, R"(
        print(test_lua.m_test_string);
        print(test_lua.m_test_int);
        test_lua:test();

        test_lua.m_test_string = "new test_lua::m_test_string";
        test_lua.m_test_int = 200;
        print(test_lua.m_test_string);
        print(test_lua.m_test_int);
    )");
}

void test_4(lua_State *L) {
    // libUnityInjector_lib.so
}
