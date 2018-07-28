#include <iostream>
#include "engine/entity-system/include.hpp"
#include "engine/utils/Profiler.hpp"

struct X {};

/*
 * No optimization flags:
 *  10^6 entities: 56ms -> 29ms
 *  10^7 entities: 502ms -> 297ms
 *
 * -O3:
 *  10^7 entities: 30ms -> 27ms
 *  10^8 entities: 304ms -> 285ms
 */
int main(int, char**) {
    using engine::entitysystem::Entity;
    engine::entitysystem::ComponentManager manager;
    engine::utils::Profiler profiler;

    manager.reserve<X>(1e7);

    profiler.start();
    for (size_t i = 0; i < 1e7; i++) {
        Entity e = manager.createEntity();
        manager.addComponent<X>(e);
    }
    profiler.finish();
    std::cout << "Elapsed time (addComponent): " <<
        profiler.measureAsMilliseconds() << "ms" << std::endl;

    size_t counter = 0;

    profiler.start();
    manager.forEachEntity<X>([&counter](Entity, X&) {
        ++counter;
    });
    profiler.finish();

    std::cout << counter << std::endl;
    std::cout << "Elapsed time (forEachEntity): " <<
        profiler.measureAsMilliseconds() << "ms" << std::endl;
}
