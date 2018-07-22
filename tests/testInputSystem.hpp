#include <unordered_set>
#include "engine/input-system/include.hpp"
#include "engine/testing/include.hpp"

using test::before;
using test::describe;
using test::it;

class RawInputMock : public engine::inputsystem::RawInput {
    using KeyboardKey = engine::inputsystem::KeyboardKey;
 public:
    void press(KeyboardKey key) {
        pressedKeys.insert(key);
    }

    void release(KeyboardKey key) {
        pressedKeys.erase(key);
    }

    void releaseAll() {
        pressedKeys.clear();
    }

 private:
    std::unordered_set<KeyboardKey> pressedKeys;

    bool isKeyPressedImpl(KeyboardKey key) const override {
        return pressedKeys.count(key);
    }
};

void testInputSystem() {
    using namespace engine::inputsystem;

    describe("input system", []() {
        const std::unordered_map<KeyboardKey, GameKey> keyMapping = {
            {KeyboardKey::A, "KA"},
            {KeyboardKey::S, "KS"},
            {KeyboardKey::D, "KD"},
        };

        RawInputMock* rawInputPtr = new RawInputMock();
        RawInputMock& rawInput = *rawInputPtr;
        InputTracker inputTracker(std::unique_ptr<RawInput>(rawInputPtr), keyMapping);
        InputDispatcher inputDispatcher(inputTracker);

        auto addBasicContext = [&]() {
            InputContext context;
            context.actions = {{"KA", "EventA"}};
            context.states = {{"KS", "EventS"}};
            inputDispatcher.registerContext("ContextA", context);
        };

        before([&]() {
            rawInput.releaseAll();
            inputDispatcher.clear();
        });

        it("reacts to actions appropriately", [&]() {
            addBasicContext();
            inputDispatcher.enableContext("ContextA");

            bool flag = false;
            inputDispatcher.addObserver([&](const EventIdentifier& event) {
                expect(event).toBe("EventA");
                flag = true;
            });

            rawInput.press(KeyboardKey::A);
            inputDispatcher.tick();
            expect(flag).toBe(true);

            flag = false;
            inputDispatcher.tick();
            expect(flag).toBe(false);
        });

        it("reacts to states appropriately", [&]() {
            addBasicContext();
            inputDispatcher.enableContext("ContextA");

            bool flag = false;
            inputDispatcher.addObserver([&](const EventIdentifier &event) {
                expect(event).toBe("EventS");
                flag = true;
            });

            rawInput.press(KeyboardKey::S);
            inputDispatcher.tick();
            expect(flag).toBe(true);

            flag = false;
            inputDispatcher.tick();
            expect(flag).toBe(true);
        });

        it("respects the priority of the contexts", [&]() {
            InputContext contextA;
            contextA.actions = {{"KA", "(A) EventA"}, {"KD", "(A) EventD"}};
            contextA.states = {{"KS", "(A) EventS"}};
            contextA.priority = 1;
            inputDispatcher.registerContext("ContextA", contextA);

            InputContext contextB;
            contextB.actions = {{"KA", "(B) EventA"}};
            contextB.priority = 2;
            inputDispatcher.registerContext("ContextB", contextB);

            InputContext contextC;
            contextC.states = {{"KS", "(C) EventS"}};
            contextC.priority = 3;
            inputDispatcher.registerContext("ContextC", contextC);

            inputDispatcher.enableContext("ContextC");
            inputDispatcher.enableContext("ContextA");
            inputDispatcher.enableContext("ContextB");

            std::unordered_set<std::string> triggeredEvents;
            inputDispatcher.addObserver([&](const EventIdentifier& event) {
                triggeredEvents.insert(event);
            });

            rawInput.press(KeyboardKey::A);
            rawInput.press(KeyboardKey::S);
            rawInput.press(KeyboardKey::D);
            inputDispatcher.tick();
            expect(triggeredEvents.size()).toBe(3);
            expect(triggeredEvents.count("(A) EventD")).toBe(1);
            expect(triggeredEvents.count("(B) EventA")).toBe(1);
            expect(triggeredEvents.count("(C) EventS")).toBe(1);

            triggeredEvents.clear();
            inputDispatcher.tick();
            expect(triggeredEvents.size()).toBe(1);
            expect(triggeredEvents.count("(C) EventS")).toBe(1);
        });
    });
}
