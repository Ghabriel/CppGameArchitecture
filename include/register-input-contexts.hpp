#ifndef REGISTER_INPUT_CONTEXTS_HPP
#define REGISTER_INPUT_CONTEXTS_HPP

#include <iostream>
#include "engine/input-system/include.hpp"

inline void registerInputContexts(engine::inputsystem::InputDispatcher& inputDispatcher) {
    using namespace engine::inputsystem;

    InputContext contextA;
    contextA.actions = {{"A", "(A) EventA"}};
    contextA.states = {{"S", "(A) EventB"}};
    contextA.priority = 5;
    inputDispatcher.registerContext("ContextA", contextA);

    InputContext contextB;
    contextB.actions = {{"A", "(B) EventA"}};
    contextB.states = {{"D", "(B) EventB"}};
    contextB.priority = 6;
    inputDispatcher.registerContext("ContextB", contextB);

    inputDispatcher.addObserver([](const EventIdentifier &event) {
        std::cout << "[TRIGGER] " << event << std::endl;
    });
    inputDispatcher.enableContext("ContextA");
    inputDispatcher.enableContext("ContextB");
}

#endif
