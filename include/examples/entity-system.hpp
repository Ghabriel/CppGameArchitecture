#include <iostream>
#include <random>
#include "../components/Health.hpp"
#include "../components/Position.hpp"
#include "../engine/core.hpp"
#include "../visitors/ComponentPrinterVisitor.hpp"

using engine::entitysystem::ComponentManager;
using engine::entitysystem::Entity;

inline void setInitialHP(ComponentManager& manager) {
    ComponentPrinterVisitor visitor;

    manager.forEachEntity<Health>([&visitor](Entity entity, Health& entityHealth) {
        entityHealth.health = 100;
        std::cout << "[setInitialHP] " << entity << " ";
        visitor.visit(entityHealth);
    });
}

inline void setInitialPosition(ComponentManager& manager) {
    ComponentPrinterVisitor visitor;

    manager.forEachEntity<Position>([&visitor](Entity entity, Position& entityPosition) {
        entityPosition.x = 5 - entity;
        entityPosition.y = 7 + entity;
        std::cout << "[setInitialPosition] " << entity << " ";
        visitor.visit(entityPosition);
    });
}

inline void deathSystem(ComponentManager& manager) {
    manager.forEachEntity<Health>([&manager](Entity entity, Health& entityHealth) {
        if (entityHealth.health <= 0) {
            std::cout << "[DEATH] " << entity << std::endl;
            manager.deleteEntity(entity);
        } else {
            std::cout << entity << " shall live... for now. ("
                << entityHealth.health << " HP left)" << std::endl;
        }
    });
}

inline void damage(ComponentManager& manager) {
    manager.forEachEntity<Health>([](Entity entity, Health& entityHealth) {
        entityHealth.health -= 60;
    });
}

template<typename T, typename... Ts, typename TVisitor>
inline void applyVisitor(ComponentManager& manager, Entity entity, TVisitor& visitor) {
    if (manager.hasComponent<T>(entity)) {
        std::cout << '\t';
        visitor.visit(manager.getData<T>(entity));
    }

    if constexpr (sizeof...(Ts) > 0) {
        applyVisitor<Ts...>(manager, entity, visitor);
    }
}

template<typename T, typename... Ts>
inline void debug(ComponentManager& manager, Entity entity) {
    std::cout << "Entity: " << entity << '\n';
    std::cout << "Components:\n";
    ComponentPrinterVisitor printer;
    applyVisitor<T, Ts...>(manager, entity, printer);
}

inline int mainEntitySystem(int, char**) {
    ComponentManager manager;

    Entity player = manager.createEntity();
    manager.addComponent<Health>(player);
    manager.addComponent<Position>(player);
    Entity player2 = manager.createEntity();
    manager.addComponent<Health>(player2);

    setInitialHP(manager);
    setInitialPosition(manager);
    debug<Health, Position>(manager, player);
    debug<Health, Position>(manager, player2);

    deathSystem(manager);
    damage(manager);
    manager.getData<Health>(player).health -= 40;
    deathSystem(manager);
    std::cout << "--------------\n";
    manager.forEachEntity<Health>([](Entity entity, Health& entityHealth) {
        std::cout << "[ENTITY] " << entity << std::endl;
    });

    manager.cleanup<Health, Position>();
    debug<Health, Position>(manager, player);
    return 0;
}
