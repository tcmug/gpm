#ifndef LUA_HPP
#define LUA_HPP

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <string>
#include "lua_binding.hpp"

namespace ton {

class lua {

    private:

        lua_State *L;

    public:

        lua();
        ~lua();
        lua_State *get_state() const { return this->L; }

        void dofile(const char *file);

        std::string get_table_string(const char *key, const char *def);
        int get_table_int(const char *key, int def);
        float get_table_float(const char *key, float def);

};

}

#endif
