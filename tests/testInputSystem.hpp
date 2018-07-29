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

        bool firedEventA = false;
        bool firedEventS = false;

        auto addBasicContext = [&]() {
            InputContext context;
            context.actions = {{"KA", [&] { firedEventA = true; }}};
            context.states = {{"KS", [&] { firedEventS = true; }}};
            inputDispatcher.registerContext("ContextA", context);
        };

        before([&]() {
            rawInput.releaseAll();
            inputDispatcher.clear();
            firedEventA = false;
            firedEventS = false;
        });

        it("reacts to actions appropriately", [&]() {
            addBasicContext();
            inputDispatcher.enableContext("ContextA");

            rawInput.press(KeyboardKey::A);
            inputDispatcher.tick();
            expect(firedEventA).toBe(true);
            expect(firedEventS).toBe(false);

            firedEventA = false;
            inputDispatcher.tick();
            expect(firedEventA).toBe(false);
            expect(firedEventS).toBe(false);
        });

        it("reacts to states appropriately", [&]() {
            addBasicContext();
            inputDispatcher.enableContext("ContextA");

            rawInput.press(KeyboardKey::S);
            inputDispatcher.tick();
            expect(firedEventA).toBe(false);
            expect(firedEventS).toBe(true);

            firedEventS = false;
            inputDispatcher.tick();
            expect(firedEventA).toBe(false);
            expect(firedEventS).toBe(true);
        });

        it("respects the priority of the contexts", [&]() {
            bool firedAA = false;
            bool firedAS = false;
            bool firedAD = false;
            bool firedBA = false;
            bool firedCS = false;

            InputContext contextA;
            contextA.actions = {
                {"KA", [&] { firedAA = true; }},
                {"KD", [&] { firedAD = true; }}
            };
            contextA.states = {
                {"KS", [&] { firedAS = true; }}
            };
            contextA.priority = 1;
            inputDispatcher.registerContext("ContextA", contextA);

            InputContext contextB;
            contextB.actions = {
                {"KA", [&] { firedBA = true; }}
            };
            contextB.priority = 2;
            inputDispatcher.registerContext("ContextB", contextB);

            InputContext contextC;
            contextC.states = {
                {"KS", [&] { firedCS = true; }}
            };
            contextC.priority = 3;
            inputDispatcher.registerContext("ContextC", contextC);

            inputDispatcher.enableContext("ContextC");
            inputDispatcher.enableContext("ContextA");
            inputDispatcher.enableContext("ContextB");

            rawInput.press(KeyboardKey::A);
            rawInput.press(KeyboardKey::S);
            rawInput.press(KeyboardKey::D);
            inputDispatcher.tick();
            expect(firedAA).toBe(false);
            expect(firedAS).toBe(false);
            expect(firedAD).toBe(true);
            expect(firedBA).toBe(true);
            expect(firedCS).toBe(true);

            firedAD = false;
            firedBA = false;
            firedCS = false;
            inputDispatcher.tick();
            expect(firedAA).toBe(false);
            expect(firedAS).toBe(false);
            expect(firedAD).toBe(false);
            expect(firedBA).toBe(false);
            expect(firedCS).toBe(true);
        });
    });
}
