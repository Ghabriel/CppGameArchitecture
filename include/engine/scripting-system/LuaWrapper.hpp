#ifndef SCRIPTING_SYSTEM_LUA_WRAPPER_HPP
#define SCRIPTING_SYSTEM_LUA_WRAPPER_HPP

#include <cassert>
#include <functional>
#include "LuaRAII.hpp"
#include "../utils/debug/xtrace.hpp"

namespace engine::scriptingsystem {
    namespace __detail {
        template<typename T>
        T get(lua_State* L);

        template<>
        inline bool get(lua_State* L) {
            return lua_toboolean(L, -1);
        }

        template<>
        inline float get(lua_State* L) {
            assert(lua_isnumber(L, -1));
            return lua_tonumber(L, -1);
        }

        template<>
        inline int get(lua_State* L) {
            assert(lua_isnumber(L, -1));
            return lua_tonumber(L, -1);
        }

        template<>
        inline std::string get(lua_State* L) {
            assert(lua_isstring(L, -1));
            return std::string(lua_tostring(L, -1));
        }
    }

    class LuaWrapper {
        template<typename Ret, typename... Args>
        using CFunction = Ret (*)(Args...);
     public:
        using LuaCFunction = CFunction<int, lua_State*>;
        using LuaFunction = std::function<int(lua_State*)>;

        LuaWrapper(const std::string& filename);

        void pushGlobal(const std::string& variableName);
        void pushField(const std::string& fieldName);
        void pop(size_t count = 1);
        template<typename T>
        void pushValue(const T&);
        void pushFunction(LuaCFunction);
        void setGlobal(const std::string& globalName);
        void call(size_t paramCount, size_t returnCount);

        bool isNil() const;
        template<typename T>
        T get() const;

        template<typename Ret, typename... Args>
        LuaCFunction createLuaFunction(CFunction<Ret, Args...>);
        // template<typename Ret, typename... Args>
        // LuaFunction createLuaFunction(std::function<Ret(Args...)>);

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

    inline void LuaWrapper::pushFunction(LuaCFunction fn) {
        // lua_pushcfunction(L.get(), *x);
        // lua_pushcfunction(L.get(), *fn.target<int (*)(lua_State*)>());
        lua_pushcfunction(L.get(), fn);
    }

    inline void LuaWrapper::setGlobal(const std::string& globalName) {
        lua_setglobal(L.get(), globalName.c_str());
    }

    inline void LuaWrapper::call(size_t paramCount, size_t returnCount) {
        lua_pcall(L.get(), paramCount, returnCount, 0);
    }

    inline bool LuaWrapper::isNil() const {
        return lua_isnil(L.get(), -1);
    }

    template<typename T>
    inline T LuaWrapper::get() const {
        return __detail::get<T>(L.get());
    }

    template<typename Ret, typename... Args>
    inline LuaWrapper::LuaCFunction LuaWrapper::createLuaFunction(CFunction<Ret, Args...> fn) {
        static auto staticFn = fn;

        struct Container {
            static int luaWrapper(lua_State* rawState) {
                staticFn(__detail::get<std::decay_t<Args>>(rawState)...);

                if constexpr (std::is_same_v<Ret, void>) {
                    return 0;
                } else {
                    return 1;
                }
            }
        };

        return &Container::luaWrapper;
    }

    // template<typename Ret, typename... Args>
    // LuaWrapper::LuaFunction LuaWrapper::createLuaFunction(std::function<Ret(Args...)> fn) {
    //     return [*this, fn](lua_State* rawState) {
    //         fn(get<Args>()...);

    //         if constexpr (std::is_same_v<Ret, void>) {
    //             return 0;
    //         } else {
    //             return 1;
    //         }
    //     };
    // }
}

#endif
