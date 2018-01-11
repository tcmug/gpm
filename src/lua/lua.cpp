
#include "lua.hpp"

using ton::lua;

lua::lua() {

    L = luaL_newstate();

    if (L) {
        printf("Lua VM started\n");
    }

    luaL_requiref(L, "io", luaopen_io, 1);
    luaL_requiref(L, "base", luaopen_base, 1);
    luaL_requiref(L, "math", luaopen_math, 1);
    luaL_requiref(L, "table", luaopen_table, 1);
    luaL_requiref(L, "string", luaopen_string, 1);

    //Luna <Foo>::Register(L);

    if (luaL_dofile(L, "preferences.lua") != 0) {
        printf("%s\n", lua_tostring(L, -1));
        return;
    }

}


lua::~lua() {
    lua_close(L);
}


void lua::dofile(const char *file) {
    // Start by loading preferences to the Lua stack.
    if (luaL_dofile(L, file) != 0) {
        printf("%s\n", lua_tostring(L, -1));
    }
}


std::string lua::get_table_string(const char *key, const char *def) {
    std::string value;
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (lua_isstring(L, -1)) {
        value = lua_tostring(L, -1);
    } else {
        value = def;
    }
    lua_pop(L, 1);
    return value;
}


int lua::get_table_int(const char *key, int def) {
    int value;
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (lua_isnumber(L, -1)) {
        value = lua_tointeger(L, -1);
    } else {
        value = def;
    }
    lua_pop(L, 1);
    return value;
}




float lua::get_table_float(const char *key, float def) {
    float value;
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (lua_isnumber(L, -1)) {
        value = lua_tonumber(L, -1);
    } else {
        value = def;
    }
    lua_pop(L, 1);
    return value;
}


ton::lua lua;
