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
        template<typename T, typename... Args>
        T call(const std::string& functionName, Args&&...);

     private:
        LuaWrapper luaState;

        size_t pushVariableToStack(const std::string& variableName);
    };

    inline Lua::Lua(const std::string& filename) : luaState(filename) { }

    template<typename T>
    inline T Lua::get(const std::string& variableName) {
        size_t level = pushVariableToStack(variableName);
        T result = luaState.get<T>();
        luaState.pop(level + 1);
        return result;
    }

    template<typename T, typename... Args>
    inline T Lua::call(const std::string& functionName, Args&&... args) {
        pushVariableToStack(functionName);
        (luaState.pushValue(std::forward<Args>(args)), ...);
        luaState.call(sizeof...(args));
        T result = luaState.get<T>();
        luaState.pop();
        return result;
    }

    inline size_t Lua::pushVariableToStack(const std::string& variableName) {
        size_t level = 0;
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
        return level;
    }
}

#endif
