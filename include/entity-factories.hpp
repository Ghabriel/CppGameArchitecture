#ifndef ENTITY_FACTORIES_HPP
#define ENTITY_FACTORIES_HPP

#include "components/CircularTraits.hpp"
#include "components/Position.hpp"
#include "engine/entity-system/include.hpp"

engine::entitysystem::Entity createCircle(engine::entitysystem::ComponentManager& manager) {
    auto entity = manager.createEntity();
    manager.addComponent<CircularTraits>(entity);
    manager.getData<CircularTraits>(entity) = {100, sf::Color::Blue};
    manager.addComponent<Position>(entity);
    manager.getData<Position>(entity) = {10, 20};
    return entity;
}

// engine::entitysystem::Entity createPlayer(engine::entitysystem::ComponentManager& manager) {
//     auto entity = manager.createEntity();
//     manager.addComponent<CircularTraits>(entity);
//     manager.getData<CircularTraits>(entity) = {100, sf::Color::Blue};
//     manager.addComponent<Position>(entity);
//     manager.getData<Position>(entity) = {10, 20};
//     return entity;
// }

#endif
