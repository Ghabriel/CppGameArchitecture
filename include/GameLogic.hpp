#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "engine/entity-system/include.hpp"
#include "engine/game-loop/include.hpp"
#include "engine/input-system/include.hpp"
#include "engine/sfml/sprite-system/include.hpp"
#include "engine/utils/print-fps.hpp"
#include "entity-factories.hpp"
#include "load-input-tracker.hpp"
#include "register-input-contexts.hpp"

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
        createPlayer(componentManager);
        registerInputContexts(inputDispatcher);
    }

    void operator()(GameLoop& game) {
        engine::utils::printFPS<1>("Update Rate", 50);
        inputDispatcher.tick();

        using namespace engine::spritesystem;
        playAnimations<LoopingAnimationData>(componentManager);
    }

 private:
    ComponentManager& componentManager;
    InputTracker inputTracker;
    InputDispatcher inputDispatcher;
};

#endif
