#ifndef INPUT_SYSTEM_INPUT_DISPATCHER_HPP
#define INPUT_SYSTEM_INPUT_DISPATCHER_HPP

#include <string>
#include <unordered_set>
#include "InputContext.hpp"
#include "InputTracker.hpp"
#include "types.hpp"

namespace engine::inputsystem {
    /**
     * Manages the active input contexts and emits their callback
     * identifiers at the appropriate moments to input observers.
     */
    class InputDispatcher {
     public:
        using Observer = std::function<void(const EventIdentifier&)>;

        InputDispatcher(InputTracker&);

        /**
         * \brief Should be called at every update tick. Updates the
         * internal structures and dispatches appropriate callback
         * identifiers to all input observers.
         */
        void tick();

        /**
         * \brief Adds an observer to this input dispatcher.
         */
        void addObserver(Observer fn);

        /**
         * \brief Registers a context, mapping it to a given name.
         */
        void registerContext(const std::string& name, const InputContext&);
        /**
         * \brief Enables a context, given its name. Throws if it's not
         * a registered name.
         */
        void enableContext(const std::string& contextName);
        /**
         * \brief Disables a context, given its name. Throws if it's not
         * a registered name.
         */
        void disableContext(const std::string& contextName);
        /**
         * \brief Disables all the enabled contexts.
         */
        void disableAllContexts();
    
     private:
        InputTracker& inputTracker;
        std::vector<Observer> observers;
        // TODO: order them by priority
        std::unordered_set<const InputContext*> activeContexts;
        std::unordered_map<std::string, const InputContext> registeredContexts;
    };

    InputDispatcher::InputDispatcher(InputTracker &tracker) : inputTracker(tracker) {}

    void InputDispatcher::tick() {
        inputTracker.tick();

        std::vector<EventIdentifier> triggeredEvents;

        for (const InputContext* context : activeContexts) {
            const auto& actions = context->actions;
            const auto& states = context->states;

            inputTracker.forEachActionKey([&](const GameKey& key) {
                if (actions.count(key)) {
                    triggeredEvents.push_back(actions.at(key));
                }
            });

            inputTracker.forEachStateKey([&](const GameKey& key) {
                if (states.count(key)) {
                    triggeredEvents.push_back(states.at(key));
                }
            });
        }

        for (const auto& observer : observers) {
            for (const auto& eventIdentifier : triggeredEvents) {
                observer(eventIdentifier);
            }
        }
    }

    void InputDispatcher::addObserver(Observer fn) {
        observers.push_back(fn);
    }

    void InputDispatcher::registerContext(
        const std::string& name,
        const InputContext& context
    ) {
        registeredContexts.insert({name, context});
    }

    void InputDispatcher::enableContext(const std::string& contextName) {
        activeContexts.insert(&registeredContexts.at(contextName));
    }

    void InputDispatcher::disableContext(const std::string& contextName) {
        activeContexts.erase(&registeredContexts.at(contextName));
    }

    void InputDispatcher::disableAllContexts() {
        activeContexts.clear();
    }
}

#endif
