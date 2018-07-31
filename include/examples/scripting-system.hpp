#include <iostream>
#include "../engine/scripting-system/Lua.hpp"

void write(const std::string& str) {
    std::cout << str << std::endl;
}

void ignore(const std::string& str) {
    std::cout << "[IGNORED]" << std::endl;
}

int mainScriptingSystem(int, char**) {
    engine::scriptingsystem::Lua script("resources/lua-test.lua");
    std::cout << "filename: " << script.get<std::string>("player.filename") << std::endl;
    std::cout << "posX: " << script.get<int>("player.pos.X") << std::endl;

    std::cout << "5 + 7 = " << script.call<int>("sum", 5, 7) << std::endl;
    std::cout << "HP: " << script.get<int>("player.HP") << std::endl;
    script.call<void>("setPlayerHP", 12);
    std::cout << "HP: " << script.get<int>("player.HP") << std::endl;

    script.registerNative("write", write);
    script.registerNative("ignore", ignore);
    script.call<void>("testCppFunction");

    return 0;
}
