#include "register-input-contexts.hpp"

#include <iostream>
#include "engine/input-system/include.hpp"

void registerInputContexts(engine::inputsystem::InputDispatcher& inputDispatcher) {
    using namespace engine::inputsystem;

    InputContext contextA;
    contextA.actions = {{"A", [] { std::cout << "(A) EventA" << std::endl; }}};
    contextA.states = {{"S", [] { std::cout << "(A) EventB" << std::endl; }}};
    contextA.priority = 5;
    inputDispatcher.registerContext("ContextA", contextA);

    InputContext contextB;
    contextB.actions = {{"A", [] { std::cout << "(B) EventA" << std::endl; }}};
    contextB.states = {{"D", [] { std::cout << "(B) EventB" << std::endl; }}};
    contextB.priority = 6;
    inputDispatcher.registerContext("ContextB", contextB);

    inputDispatcher.enableContext("ContextA");
    inputDispatcher.enableContext("ContextB");
}
