#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>
#include "engine/entity-system/include.hpp"
#include "engine/game-loop/include.hpp"
#include "../GameLogic.hpp"
#include "../Renderer.hpp"

int mainGameLoop(int, char**) {
    using engine::gameloop::GameLoop;
    XInitThreads();
    engine::entitysystem::ComponentManager componentManager;
    engine::resourcesystem::ResourceStorage resourceStorage;
    GameLogic logic(componentManager, resourceStorage);
    Renderer renderer(componentManager, resourceStorage);

    GameLoop gameLoop(
        [&](GameLoop& loop) { logic(loop); },
        std::move(renderer)
    );
    gameLoop.start();
    gameLoop.join();

    return 0;
}
