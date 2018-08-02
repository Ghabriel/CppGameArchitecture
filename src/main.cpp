// #include "examples/entity-system.hpp"
// #include "examples/single-thread-game-loop.hpp"
// #include "examples/multi-thread-game-loop.hpp"
#include "examples/scripting-system.hpp"
// #include "examples/state-system.hpp"

int main(int argc, char** argv) {
    // return mainEntitySystem(argc, argv);
    // return mainGameLoop(argc, argv);
    return mainScriptingSystem(argc, argv);
    // return mainStateSystem(argc, argv);
}
