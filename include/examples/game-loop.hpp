#include "engine/entity-system/include.hpp"
#include "engine/game-loop/include.hpp"
#include "../GameLogic.hpp"
#include "../Renderer.hpp"

int mainGameLoop(int, char**) {
    engine::entitysystem::ComponentManager componentManager;
    GameLogic logic(componentManager);
    Renderer renderer(componentManager);

    engine::gameloop::GameLoop gameLoop(logic, std::move(renderer));
    gameLoop.start();
    gameLoop.join();

    return 0;
}
