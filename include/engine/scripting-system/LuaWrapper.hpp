#ifndef SCRIPTING_SYSTEM_LUA_WRAPPER_HPP
#define SCRIPTING_SYSTEM_LUA_WRAPPER_HPP

#include <cassert>
#include "LuaRAII.hpp"

namespace engine::scriptingsystem {
    class LuaWrapper {
     public:
        LuaWrapper(const std::string& filename);

        void pushGlobal(const std::string& variableName);
        void pushField(const std::string& fieldName);
        void pop(size_t count = 1);
        template<typename T>
        void pushValue(const T&);
        void call(size_t paramCount);

        bool isNil() const;
        template<typename T>
        T get() const;

     private:
        LuaRAII L;
    };

    LuaWrapper::LuaWrapper(const std::string& filename) : L(filename) { }

    inline void LuaWrapper::pushGlobal(const std::string& variableName) {
        lua_getglobal(L.get(), variableName.c_str());
    }

    inline void LuaWrapper::pushField(const std::string& fieldName) {
        lua_getfield(L.get(), -1, fieldName.c_str());
    }

    inline void LuaWrapper::pop(size_t count) {
        lua_pop(L.get(), count);
    }

    template<>
    inline void LuaWrapper::pushValue(const bool& value) {
        lua_pushboolean(L.get(), value);
    }

    template<>
    inline void LuaWrapper::pushValue(const float& value) {
        lua_pushnumber(L.get(), value);
    }

    template<>
    inline void LuaWrapper::pushValue(const int& value) {
        lua_pushnumber(L.get(), value);
    }

    template<>
    inline void LuaWrapper::pushValue(const std::string& value) {
        lua_pushstring(L.get(), value.c_str());
    }

    inline void LuaWrapper::call(size_t paramCount) {
        lua_pcall(L.get(), paramCount, 1, 0);
    }

    inline bool LuaWrapper::isNil() const {
        return lua_isnil(L.get(), -1);
    }

    template<>
    inline bool LuaWrapper::get() const {
        return lua_toboolean(L.get(), -1);
    }

    template<>
    inline float LuaWrapper::get() const {
        assert(lua_isnumber(L.get(), -1));
        return lua_tonumber(L.get(), -1);
    }

    template<>
    inline int LuaWrapper::get() const {
        assert(lua_isnumber(L.get(), -1));
        return lua_tonumber(L.get(), -1);
    }

    template<>
    inline std::string LuaWrapper::get() const {
        assert(lua_isstring(L.get(), -1));
        return std::string(lua_tostring(L.get(), -1));
    }
}

#endif
