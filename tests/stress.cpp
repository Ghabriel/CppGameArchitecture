#include <iostream>
#include "engine/entity-system/include.hpp"
#include "engine/utils/Profiler.hpp"

struct X {};

int main(int, char**) {
    using engine::entitysystem::Entity;
    engine::entitysystem::ComponentManager manager;
    engine::utils::Profiler profiler;

    for (size_t i = 0; i < 1e6; i++) {
        Entity e = manager.createEntity();
        manager.addComponent<X>(e);
    }

    size_t counter = 0;

    profiler.start();
    manager.forEachEntity<X>([&counter](Entity, X&) {
        ++counter;
    });
    profiler.finish();

    std::cout << counter << std::endl;
    std::cout << "Elapsed time: " << profiler.measureAsMilliseconds() << "ms" << std::endl;
}
