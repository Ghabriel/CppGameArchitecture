#ifndef SCRIPTING_SYSTEM_LUA_HPP
#define SCRIPTING_SYSTEM_LUA_HPP

#include <cassert>
#include <iostream>
#include <vector>
#include "LuaWrapper.hpp"

namespace engine::scriptingsystem {
    class Lua {
     public:
        Lua(const std::string& filename);

        template<typename T>
        T get(const std::string& variableName);

     private:
        LuaWrapper luaState;
        int level;

        void pushValueToStack(const std::string& variableName);
    };

    inline Lua::Lua(const std::string& filename) : luaState(filename) { }

    template<typename T>
    inline T Lua::get(const std::string& variableName) {
        pushValueToStack(variableName);
        T result = luaState.get<T>();
        luaState.pop(level + 1);
        return result;
    }

    inline void Lua::pushValueToStack(const std::string& variableName) {
        level = 0;
        std::string var;

        for (char ch : variableName) {
            if (ch != '.') {
                var += ch;
                continue;
            }
            
            if (level == 0) {
                luaState.pushGlobal(var);
            } else {
                luaState.pushField(var);
            }

            assert(!luaState.isNil());
            var = "";
            ++level;
        }

        if (level == 0) {
            luaState.pushGlobal(var);
        } else {
            luaState.pushField(var);
        }

        assert(!luaState.isNil());
    }
}

#endif
