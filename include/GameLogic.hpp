#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "engine/entity-system/include.hpp"
#include "engine/game-loop/include.hpp"
#include "engine/input-system/include.hpp"
#include "engine/resource-system/ResourceStorage.hpp"
#include "engine/sfml/sprite-system/include.hpp"
#include "engine/state-system/include.hpp"
#include "entity-factories.hpp"
#include "load-input-tracker.hpp"
#include "load-resources.hpp"
#include "register-input-contexts.hpp"
#include "register-states.hpp"

class GameLogic {
    using ComponentManager = engine::entitysystem::ComponentManager;
    using Entity = engine::entitysystem::Entity;
    using EventIdentifier = engine::inputsystem::EventIdentifier;
    using GameLoop = engine::gameloop::GameLoop;
    using InputContext = engine::inputsystem::InputContext;
    using InputDispatcher = engine::inputsystem::InputDispatcher;
    using InputTracker = engine::inputsystem::InputTracker;
    using ResourceStorage = engine::resourcesystem::ResourceStorage;
    using StateMachine = engine::statesystem::StateMachine;
 public:
    GameLogic(ComponentManager& manager)
     : componentManager(manager),
       inputTracker(loadInputTracker("resources/controls.json")),
       inputDispatcher(inputTracker),
       stateMachine(std::make_unique<StateMachine>()) {
        loadResources(resourceStorage);
        registerInputContexts(inputDispatcher);
        registerStates(*stateMachine);
        stateMachine->pushState("menu-state");
    }

    void operator()(GameLoop& game) {
        inputDispatcher.tick();
        stateMachine->execute();

        // using namespace engine::spritesystem;
        // playAnimations<LoopingAnimationData>(componentManager);
    }

 private:
    ComponentManager& componentManager;
    InputTracker inputTracker;
    InputDispatcher inputDispatcher;
    ResourceStorage resourceStorage;
    std::shared_ptr<StateMachine> stateMachine;
};

#endif
