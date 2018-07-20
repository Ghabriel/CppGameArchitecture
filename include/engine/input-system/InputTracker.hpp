#ifndef INPUT_SYSTEM_INPUT_TRACKER_HPP
#define INPUT_SYSTEM_INPUT_TRACKER_HPP

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "KeyboardKey.hpp"
#include "RawInput.hpp"
#include "types.hpp"

namespace engine::inputsystem {
    /**
     * \brief Stores the necessary information to tell whether a keypress
     * is an action or a state.
     */
    class InputTracker {
     public:
        /**
         * \brief Constructs an InputTracker with a given RawInput to
         * check for pressed keys and a mapping between relevant keyboard
         * keys and their contextualized names ("game keys").
         */
        InputTracker(
            std::shared_ptr<RawInput>,
            const std::unordered_map<KeyboardKey, GameKey>&
        );

        /**
         * \brief Updates the internal structures regarding which keys are
         * being continuously pressed (states) and which weren't being pressed
         * but now are (actions).
         */
        void tick();

        /**
         * \brief Calls a function for every game key that is currently an action.
         */
        void forEachActionKey(std::function<void(const GameKey&)> fn) const;
        /**
         * \brief Calls a function for every game key that is currently a state.
         */
        void forEachStateKey(std::function<void(const GameKey&)> fn) const;

     private:
        std::shared_ptr<RawInput> rawInput;
        std::unordered_map<KeyboardKey, GameKey> relevantKeys;
        std::unordered_set<KeyboardKey> currentlyPressed;
        std::unordered_set<GameKey> actions;
        std::unordered_set<GameKey> states;
    };

    InputTracker::InputTracker(
        std::shared_ptr<RawInput> rawInput,
        const std::unordered_map<KeyboardKey, GameKey>& relevantKeys
    ) : rawInput(std::move(rawInput)), relevantKeys(relevantKeys) { }

    void InputTracker::tick() {
        for (const auto& [keyboardKey, gameKey] : relevantKeys) {
            if (rawInput->isKeyPressed(keyboardKey)) {
                if (currentlyPressed.count(keyboardKey)) {
                    states.insert(gameKey);
                } else {
                    actions.insert(gameKey);
                }

                currentlyPressed.insert(keyboardKey);
            } else {
                currentlyPressed.erase(keyboardKey);
            }
        }
    }

    void InputTracker::forEachActionKey(std::function<void(const GameKey&)> fn) const {
        for (const auto& gameKey : actions) {
            fn(gameKey);
        }
    }

    void InputTracker::forEachStateKey(std::function<void(const GameKey&)> fn) const {
        for (const auto& gameKey : states) {
            fn(gameKey);
        }
    }
}

#endif
