#include "register-input-contexts.hpp"

#include <iostream>
#include "engine/input-system/include.hpp"

void registerInputContexts(engine::inputsystem::InputDispatcher& inputDispatcher) {
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
