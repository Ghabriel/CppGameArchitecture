#include "register-states.hpp"

#include "engine/state-system/include.hpp"
#include "states/MenuState.hpp"

void registerStates(engine::statesystem::StateMachine& stateMachine) {
    stateMachine.registerState("menu-state", std::make_unique<MenuState>());
}
