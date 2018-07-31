#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>
#include "engine/entity-system/include.hpp"
#include "engine/game-loop/SingleThreadGameLoop.hpp"
#include "../GameLogic.hpp"
#include "../Renderer.hpp"

int mainGameLoop(int, char**) {
    using GameLoop = engine::gameloop::SingleThreadGameLoop;
    XInitThreads();
    engine::entitysystem::ComponentManager componentManager;
    engine::resourcesystem::ResourceStorage resourceStorage;
    GameLogic logic(componentManager, resourceStorage);
    Renderer renderer(componentManager, resourceStorage);

    GameLoop gameLoop(
        [&](GameLoop& loop, double elapsedTime) { logic(loop, elapsedTime); },
        [&](GameLoop& loop) { renderer(loop); }
    );
    gameLoop.start();
    gameLoop.join();

    return 0;
}
