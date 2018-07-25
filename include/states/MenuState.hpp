#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "../engine/state-system/State.hpp"
#include "../engine/utils/print-fps.hpp"
#include "../engine/utils/xtrace.hpp"

class MenuState : public engine::statesystem::State {
 private:
    void executeImpl() override {
        engine::utils::printFPS<1>("Menu Update Rate", 50);
    }
};

#endif
