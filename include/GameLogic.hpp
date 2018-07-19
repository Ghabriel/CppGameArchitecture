#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "components/CircularTraits.hpp"
#include "components/Position.hpp"
#include "engine/entity-system/include.hpp"
#include "engine/game-loop/include.hpp"
#include "engine/utils/print-fps.hpp"

class GameLogic {
    using ComponentManager = engine::entitysystem::ComponentManager;
    using Entity = engine::entitysystem::Entity;
    using GameLoop = engine::gameloop::GameLoop;
 public:
    GameLogic(ComponentManager& manager) : componentManager(manager) {
        Entity test = manager.createEntity();
        manager.addComponent<CircularTraits>(test);
        manager.getData<CircularTraits>(test) = {100, sf::Color::Blue};
        manager.addComponent<Position>(test);
        manager.getData<Position>(test) = {10, 20};
    }

    void operator()(GameLoop& game) {
        engine::utils::printFPS<1>("Update Rate", 50);
    }

 private:
    ComponentManager& componentManager;
};

#endif
