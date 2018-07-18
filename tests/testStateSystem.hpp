#include <iostream>
#include <memory>
#include "engine/state-system/include.hpp"
#include "engine/testing/include.hpp"

using test::before;
using test::describe;
using test::it;
using test::success;

class StateA : public engine::statesystem::State {
 public:
    StateA(int& ref) : ref(ref) { }

 private:
    int& ref;

    void executeImpl() override {
        ++ref;
    }
};

class StateB : public engine::statesystem::State {
 public:
    StateB(int& enterRef, int& exitRef) : enterRef(enterRef), exitRef(exitRef) { }

 private:
    int& enterRef;
    int& exitRef;

    void executeImpl() override { }

    void onEnterImpl() override {
        ++enterRef;
    }

    void onExitImpl() override {
        ++exitRef;
    }
};

void testStateSystem() {
    using namespace engine::statesystem;

    describe("state system", []() {
        StateMachine stateMachine;

        before([&]() {
            stateMachine.clear();
        });

        it("executes states correctly", [&]() {
            int countA = 0;
            int countB = 0;
            stateMachine.registerState("A", std::make_unique<StateA>(countA));
            stateMachine.registerState("B", std::make_unique<StateA>(countB));

            stateMachine.pushState("A");
            stateMachine.execute();
            stateMachine.pushState("B");
            stateMachine.execute();
            stateMachine.popState();
            stateMachine.execute();

            expect(countA).toBe(2);
            expect(countB).toBe(1);
        });

        it("executes onEnter/onExit correctly", [&]() {
            int countEnterA = 0;
            int countExitA = 0;
            int countEnterB = 0;
            int countExitB = 0;
            stateMachine.registerState(
                "A",
                std::make_unique<StateB>(countEnterA, countExitA)
            );
            stateMachine.registerState(
                "B",
                std::make_unique<StateB>(countEnterB, countExitB)
            );

            stateMachine.pushState("A");
            stateMachine.execute();
            expect(countEnterA).toBe(1);
            expect(countExitA).toBe(0);
            expect(countEnterB).toBe(0);
            expect(countExitB).toBe(0);

            stateMachine.pushState("B");
            stateMachine.execute();
            expect(countEnterA).toBe(1);
            expect(countExitA).toBe(1);
            expect(countEnterB).toBe(1);
            expect(countExitB).toBe(0);

            stateMachine.popState();
            stateMachine.execute();
            expect(countEnterA).toBe(2);
            expect(countExitA).toBe(1);
            expect(countEnterB).toBe(1);
            expect(countExitB).toBe(1);
        });

        it("doesn't throw when popState() is called on an empty state machine", [&]() {
            stateMachine.popState();
            stateMachine.popState();
            stateMachine.execute();
            success();
        });
    });
}
