#include <iostream>
#include "../engine/scripting-system/Lua.hpp"

int mainScriptingSystem(int, char**) {
    engine::scriptingsystem::Lua script("resources/lua-test.lua");
    std::string filename = script.get<std::string>("player.filename");
    int posX = script.get<int>("player.pos.X");
    std::cout << "filename: " << filename << std::endl;
    std::cout << "posX: " << posX << std::endl;

    return 0;
}
