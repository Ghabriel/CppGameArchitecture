#include <iostream>
#include "../engine/scripting-system/Lua.hpp"

int mainScriptingSystem(int, char**) {
    engine::scriptingsystem::Lua script("resources/lua-test.lua");
    std::cout << "filename: " << script.get<std::string>("player.filename") << std::endl;
    std::cout << "posX: " << script.get<int>("player.pos.X") << std::endl;
    std::cout << "5 + 7 = " << script.call<int>("sum", 5, 7) << std::endl;

    return 0;
}
