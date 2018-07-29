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

        template<typename Functor>
        void registerNative(const std::string& luaFunctionName, Functor fn);

     private:
        LuaWrapper luaState;

        size_t pushVariableValue(const std::string& variableName);
        void pushGlobalOrField(const std::string& value, size_t level);
    };

    inline Lua::Lua(const std::string& filename) : luaState(filename) { }

    template<typename T>
    inline T Lua::get(const std::string& variableName) {
        size_t level = pushVariableValue(variableName);
        T result = luaState.get<T>();
        luaState.pop(level + 1);
        return result;
    }

    template<typename T, typename... Args>
    inline T Lua::call(const std::string& functionName, Args&&... args) {
        luaState.pushGlobal(functionName);
        (luaState.pushValue(std::forward<Args>(args)), ...);

        if constexpr (!std::is_same_v<T, void>) {
            luaState.call(sizeof...(args), 1);
            T result = luaState.get<T>();
            luaState.pop();
            return result;
        } else {
            luaState.call(sizeof...(args), 0);
        }
    }

    template <typename Functor>
    inline void Lua::registerNative(const std::string& luaFunctionName, Functor fn) {
        luaState.pushFunction(fn);
        luaState.setGlobal(luaFunctionName);
    }

    inline size_t Lua::pushVariableValue(const std::string& variableName) {
        size_t level = 0;
        std::string var;

        for (char ch : variableName) {
            if (ch != '.') {
                var += ch;
                continue;
            }

            pushGlobalOrField(var, level);
            var = "";
            ++level;
        }

        pushGlobalOrField(var, level);
        return level;
    }

    inline void Lua::pushGlobalOrField(const std::string& value, size_t level) {
        if (level == 0) {
            luaState.pushGlobal(value);
        } else {
            luaState.pushField(value);
        }

        assert(!luaState.isNil());
    }
}

#endif
