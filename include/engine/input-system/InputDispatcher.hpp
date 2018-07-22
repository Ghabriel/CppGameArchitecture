#ifndef INPUT_SYSTEM_INPUT_DISPATCHER_HPP
#define INPUT_SYSTEM_INPUT_DISPATCHER_HPP

#include <set>
#include <string>
#include <unordered_set>
#include "InputContext.hpp"
#include "InputTracker.hpp"
#include "types.hpp"

namespace engine::inputsystem {
    namespace __detail {
        struct InputContextPointerCompare {
            constexpr bool operator()(
                const InputContext* lhs,
                const InputContext* rhs
            ) const {
                return lhs->priority > rhs->priority;
            }
        };
    }

    /**
     * Manages the active input contexts and emits their callback
     * identifiers at the appropriate moments to input observers.
     */
    class InputDispatcher {
        using InputContextPointerCompare = __detail::InputContextPointerCompare;
     public:
        using Observer = std::function<void(const EventIdentifier&)>;

        explicit InputDispatcher(InputTracker&);

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
        std::set<const InputContext*, InputContextPointerCompare> activeContexts;
        std::unordered_map<std::string, const InputContext> registeredContexts;
    };

    InputDispatcher::InputDispatcher(InputTracker &tracker) : inputTracker(tracker) {}

    void InputDispatcher::tick() {
        inputTracker.tick();

        std::unordered_map<GameKey, EventIdentifier> triggeredEvents;

        for (const InputContext* context : activeContexts) {
            const auto& actions = context->actions;
            const auto& states = context->states;

            inputTracker.forEachActionKey([&](const GameKey& key) {
                if (actions.count(key)) {
                    triggeredEvents.insert({key, actions.at(key)});
                }
            });

            inputTracker.forEachStateKey([&](const GameKey& key) {
                if (states.count(key)) {
                    triggeredEvents.insert({key, states.at(key)});
                }
            });
        }

        for (const auto& [_, eventIdentifier] : triggeredEvents) {
            std::cout << "[TRIGGER] " << eventIdentifier << std::endl;
            for (const auto& observer : observers) {
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
