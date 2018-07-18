#include "engine/entity-system/core.hpp"
#include "engine/testing/core.hpp"

using test::before;
using test::describe;
using test::it;

struct Component1 {
    int field1;
    std::string field2;
};

struct Component2 { };
struct Component3 { };

template<typename T, typename... Ts>
size_t countComponents(
    engine::entitysystem::ComponentManager& manager,
    engine::entitysystem::Entity entity
) {
    size_t add = manager.hasComponent<T>(entity) ? 1 : 0;

    if constexpr (sizeof...(Ts) > 0) {
        return add + countComponents<Ts...>(manager, entity);
    } else {
        return add;
    }
}

void testEntitySystem() {
    using namespace engine::entitysystem;

    describe("entity system", []() {
        ComponentManager manager;

        before([&]() {
            manager.clearAll<Component1, Component2, Component3>();
        });

        it("creates entities sequentially", [&]() {
            Entity e1 = manager.createEntity();
            Entity e2 = manager.createEntity();
            expect(e2).toBe(e1 + 1);
        });

        it("correctly stores component data", [&]() {
            Entity e1 = manager.createEntity();
            manager.addComponent<Component1>(e1);
            Component1& data1 = manager.getData<Component1>(e1);
            data1.field1 = 1;
            data1.field2 = "abc";

            Entity e2 = manager.createEntity();
            manager.addComponent<Component1>(e2);
            Component1& data2 = manager.getData<Component1>(e2);
            data2.field1 = 2;
            data2.field2 = "def";

            manager.forEachEntity<Component1>([e1](Entity entity, Component1& data) {
                if (entity == e1) {
                    expect(data.field1).toBe(1);
                    expect(data.field2).toBe("abc");
                } else {
                    expect(data.field1).toBe(2);
                    expect(data.field2).toBe("def");
                }
            });
        });

        it("filters entities by their components", [&]() {
            Entity e1 = manager.createEntity();
            manager.addComponent<Component1>(e1);
            Entity e2 = manager.createEntity();
            manager.addComponent<Component1>(e2);
            manager.addComponent<Component2>(e2);
            manager.addComponent<Component3>(e2);
            Entity e3 = manager.createEntity();
            manager.addComponent<Component1>(e3);
            manager.addComponent<Component2>(e3);

            size_t count1 = 0;
            manager.forEachEntity<Component1>([&](Entity, Component1&) { ++count1; });
            size_t count2 = 0;
            manager.forEachEntity<Component2>([&](Entity, Component2&) { ++count2; });
            size_t count3 = 0;
            manager.forEachEntity<Component3>([&](Entity, Component3&) { ++count3; });
            size_t count4 = 0;
            manager.forEachEntity<Component1, Component2>(
                [&](Entity, Component1&, Component2&) { ++count4; }
            );
            size_t count5 = 0;
            manager.forEachEntity<Component2, Component3>(
                [&](Entity, Component2&, Component3&) { ++count5; }
            );

            expect(count1).toBe(3);
            expect(count2).toBe(2);
            expect(count3).toBe(1);
            expect(count4).toBe(2);
            expect(count5).toBe(1);
        });

        it("ignores adding a component twice", [&]() {
            Entity e1 = manager.createEntity();
            manager.addComponent<Component1>(e1);
            manager.getData<Component1>(e1) = {42, "answer"};
            manager.addComponent<Component1>(e1);

            size_t count = 0;
            manager.forEachEntity<Component1>([&](Entity, Component1& data) {
                ++count;
                expect(data.field1).toBe(42);
                expect(data.field2).toBe("answer");
            });

            expect(count).toBe(1);
        });

        it("removes components correctly", [&]() {
            Entity e1 = manager.createEntity();
            expect(countComponents<Component1>(manager, e1)).toBe(0);
            manager.addComponent<Component1>(e1);
            expect(countComponents<Component1>(manager, e1)).toBe(1);
            expect(manager.hasComponent<Component1>(e1)).toBe(true);
            manager.removeComponent<Component1>(e1);
            expect(countComponents<Component1>(manager, e1)).toBe(0);
            expect(manager.hasComponent<Component1>(e1)).toBe(false);
        });

        it("removes entities correctly", [&]() {
            Entity e1 = manager.createEntity();
            manager.addComponent<Component1>(e1);
            manager.deleteEntity(e1);
            Entity e2 = manager.createEntity();
            manager.addComponent<Component1>(e2);

            manager.forEachEntity<Component1>([e2](Entity entity, Component1&) {
                expect(entity).toBe(e2);
            });

            expect(countComponents<Component1>(manager, e1)).toBe(1);
            manager.cleanup<Component1>();
            expect(countComponents<Component1>(manager, e1)).toBe(0);
        });
    });
}
