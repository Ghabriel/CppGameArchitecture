#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "components/CircularTraits.hpp"
#include "components/Position.hpp"
#include "engine/entity-system/include.hpp"
#include "engine/game-loop/include.hpp"
#include "engine/input-system/include.hpp"
#include "engine/utils/print-fps.hpp"
#include "load-input-dispatcher.hpp"

class GameLogic {
    using ComponentManager = engine::entitysystem::ComponentManager;
    using Entity = engine::entitysystem::Entity;
    using EventIdentifier = engine::inputsystem::EventIdentifier;
    using GameLoop = engine::gameloop::GameLoop;
    using InputContext = engine::inputsystem::InputContext;
    using InputDispatcher = engine::inputsystem::InputDispatcher;
    using InputTracker = engine::inputsystem::InputTracker;
 public:
    GameLogic(ComponentManager& manager)
     : componentManager(manager),
       inputTracker(loadInputTracker("resources/controls.json")),
       inputDispatcher(inputTracker) {
        Entity test = manager.createEntity();
        manager.addComponent<CircularTraits>(test);
        manager.getData<CircularTraits>(test) = {100, sf::Color::Blue};
        manager.addComponent<Position>(test);
        manager.getData<Position>(test) = {10, 20};

        InputContext contextA;
        contextA.actions = { {"A", "(A) EventA"} };
        contextA.states = { {"S", "(A) EventB"} };
        contextA.priority = 5;
        inputDispatcher.registerContext("ContextA", contextA);

        InputContext contextB;
        contextB.actions = { {"A", "(B) EventA"} };
        contextB.states = { {"D", "(B) EventB"} };
        contextB.priority = 6;
        inputDispatcher.registerContext("ContextB", contextB);

        inputDispatcher.addObserver([](const EventIdentifier& event) {
            std::cout << "[TRIGGER] " << event << std::endl;
        });
        inputDispatcher.enableContext("ContextA");
        inputDispatcher.enableContext("ContextB");
    }

    void operator()(GameLoop& game) {
        engine::utils::printFPS<1>("Update Rate", 50);
        inputDispatcher.tick();
    }

 private:
    ComponentManager& componentManager;
    InputTracker inputTracker;
    InputDispatcher inputDispatcher;
};

#endif
