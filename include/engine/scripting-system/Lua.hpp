#ifndef SCRIPTING_SYSTEM_LUA_HPP
#define SCRIPTING_SYSTEM_LUA_HPP

#include <iostream>
#include <vector>

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

namespace engine::scriptingsystem {
    class Lua {
     public:
        Lua(const std::string& filename);
        ~Lua();
        void printError(const std::string& variableName, const std::string& reason);

        template<typename T>
        T get(const std::string& variableName);
        bool lua_gettostack(const std::string& variableName);

        // Generic get
        template<typename T>
        T lua_get(const std::string& variableName) {
            return 0;
        }

        // Generic default get
        template<typename T>
        T lua_getdefault() {
            return 0;
        }

     private:
        lua_State* L;
        int level;
    };

    inline Lua::Lua(const std::string& filename) {
        L = luaL_newstate();
        if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
            std::cout << "Error: script not loaded (" << filename << ")" << std::endl;
            L = nullptr;
        }
    }

    inline Lua::~Lua() {
        if (L) lua_close(L);
    }

    void Lua::printError(const std::string& variableName, const std::string& reason) {
        std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
    }

    template<typename T>
    T Lua::get(const std::string& variableName) {
        if (!L) {
            printError(variableName, "Script is not loaded");
            return lua_getdefault<T>();
        }

        T result;
        if (lua_gettostack(variableName)) { // variable succesfully on top of stack
            result = lua_get<T>(variableName);
        } else {
            result = lua_getdefault<T>();
        }

        lua_pop(L, level + 1); // pop all existing elements from stack
        return result;
    }

    bool Lua::lua_gettostack(const std::string& variableName) {
        level = 0;
        std::string var = "";
        for(unsigned int i = 0; i < variableName.size(); i++) {
            if (variableName.at(i) == '.') {
                if (level == 0) {
                    lua_getglobal(L, var.c_str());
                } else {
                    lua_getfield(L, -1, var.c_str());
                }

                if (lua_isnil(L, -1)) {
                    printError(variableName, var + " is not defined");
                    return false;
                } else {
                    var = "";
                    level++;
                }
            } else {
                var += variableName.at(i);
            }
        }
        if (level == 0) {
            lua_getglobal(L, var.c_str());
        } else {
            lua_getfield(L, -1, var.c_str());
        }
        if (lua_isnil(L, -1)) {
            printError(variableName, var + " is not defined");
            return false;
        }

        return true;
    }

    template<>
    std::string Lua::lua_getdefault() {
        return "null";
    }

    template <>
    inline bool Lua::lua_get(const std::string& variableName) {
        return (bool)lua_toboolean(L, -1);
    }

    template <>
    inline float Lua::lua_get(const std::string& variableName) {
        if (!lua_isnumber(L, -1)) {
            printError(variableName, "Not a number");
        }
        return (float)lua_tonumber(L, -1);
    }

    template <>
    inline int Lua::lua_get(const std::string& variableName) {
        if (!lua_isnumber(L, -1)) {
            printError(variableName, "Not a number");
        }
        return (int)lua_tonumber(L, -1);
    }

    template <>
    inline std::string Lua::lua_get(const std::string& variableName) {
        std::string s = "null";
        if (lua_isstring(L, -1)) {
            s = std::string(lua_tostring(L, -1));
        } else {
            printError(variableName, "Not a string");
        }
        return s;
    }
}

#endif
