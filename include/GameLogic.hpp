#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <iostream>
#include "engine/game-loop/include.hpp"
#include "engine/utils/print-fps.hpp"

class GameLogic {
    using GameLoop = engine::gameloop::GameLoop;
 public:
    void operator()(GameLoop& game) {
        engine::utils::printFPS<1>("Update Rate", 50);
    }
};

#endif
