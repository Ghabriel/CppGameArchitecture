#ifndef GAME_CORE_DATA_HPP
#define GAME_CORE_DATA_HPP

#include "engine/entity-system/forward-declarations.hpp"
#include "engine/input-system/forward-declarations.hpp"
#include "engine/resource-system/forward-declarations.hpp"
#include "engine/state-system/forward-declarations.hpp"

struct GameCoreData {
    engine::entitysystem::ComponentManager* componentManager;
    engine::inputsystem::InputTracker* inputTracker;
    engine::inputsystem::InputDispatcher* inputDispatcher;
    engine::resourcesystem::ResourceStorage* resourceStorage;
    engine::statesystem::StateMachine* stateMachine;
};

#endif
