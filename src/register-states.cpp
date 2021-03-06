#include "register-states.hpp"

#include <memory>
#include "engine/state-system/include.hpp"
#include "states/MenuState.hpp"

void registerStates(GameCoreData& gameData) {
    gameData.stateMachine->registerState(
        "menu-state",
        std::make_unique<MenuState>(gameData)
    );
}
