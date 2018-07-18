#ifndef STATE_SYSTEM_STATE_MACHINE_HPP
#define STATE_SYSTEM_STATE_MACHINE_HPP

#include <functional>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include "NullState.hpp"
#include "State.hpp"

namespace engine::statesystem {
    class StateMachine {
     public:
        StateMachine();

        void registerState(const std::string& name, std::unique_ptr<State>);
        void pushState(const std::string& stateName);
        void popState();
        void execute() const;

     private:
        std::stack<State*> states;
        std::unordered_map<std::string, std::unique_ptr<State>> registeredStates;
    };

    StateMachine::StateMachine() {
        registeredStates.insert({"", std::make_unique<NullState>()});
        states.push(registeredStates.at("").get());
    }

    void StateMachine::registerState(
        const std::string& name,
        std::unique_ptr<State> state
    ) {
        registeredStates.insert({name, std::move(state)});
    }

    void StateMachine::pushState(const std::string& stateName) {
        states.top()->onExit();
        states.push(registeredStates.at(stateName).get());
        states.top()->onEnter();
    }

    void StateMachine::popState() {
        if (states.size() > 1) {
            states.top()->onExit();
            states.pop();
            states.top()->onEnter();
        }
    }

    void StateMachine::execute() const {
        states.top()->execute();
    }
}

#endif
