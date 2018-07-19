#include "engine/game-loop/include.hpp"
#include "../GameLogic.hpp"
#include "../Renderer.hpp"

int mainGameLoop(int, char**) {
    GameLogic logic;
    Renderer renderer;

    engine::gameloop::GameLoop gameLoop(logic, std::move(renderer));
    gameLoop.start();
    gameLoop.join();

    return 0;
}
