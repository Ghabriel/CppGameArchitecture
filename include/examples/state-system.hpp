#include <iostream>
#include <memory>
#include "../engine/state-system/include.hpp"

using engine::statesystem::State;
using engine::statesystem::StateMachine;

class SimpleState : public State {
 public:
    SimpleState(int id) : id(id) { }

 private:
    int id;

    void executeImpl() override {
        std::cout << "Execute " << id << std::endl;
    }

    void onEnterImpl() override {
        std::cout << "onEnter " << id << std::endl;
    }

    void onExitImpl() override {
        std::cout << "onExit " << id << std::endl;
    }
};

class StateChanger : public State {
 public:
    StateChanger(StateMachine& machine) : machine(machine) { }

    void executeImpl() override {
        std::cout << "Changing the current state..." << std::endl;
        machine.pushState("A");
    }

    void onEnterImpl() override {
        std::cout << "onEnter StateChanger" << std::endl;
    }

    void onExitImpl() override {
        std::cout << "onExit StateChanger" << std::endl;
    }

 private:
    StateMachine& machine;
};

int mainStateSystem(int, char**) {
    StateMachine stateMachine;

    stateMachine.registerState("A", std::make_unique<SimpleState>(1));
    stateMachine.registerState("B", std::make_unique<SimpleState>(2));
    stateMachine.registerState("C", std::make_unique<StateChanger>(stateMachine));
    stateMachine.pushState("A");
    stateMachine.execute();
    stateMachine.execute();
    stateMachine.pushState("B");
    stateMachine.execute();
    stateMachine.popState();
    stateMachine.execute();
    stateMachine.popState();
    stateMachine.execute();
    stateMachine.execute();

    stateMachine.pushState("C");
    stateMachine.execute();
    stateMachine.execute();

    return 0;
}
