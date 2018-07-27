#ifndef ENTITY_FACTORIES_HPP
#define ENTITY_FACTORIES_HPP

#include <cassert>
#include "components/CircularTraits.hpp"
#include "components/Position.hpp"
#include "engine/entity-system/include.hpp"
#include "engine/resource-system/ResourceStorage.hpp"
#include "engine/sfml/sprite-system/include.hpp"

engine::entitysystem::Entity createCircle(engine::entitysystem::ComponentManager& manager) {
    auto entity = manager.createEntity();
    manager.addComponent<CircularTraits>(entity, {100, sf::Color::Blue});
    manager.addComponent<Position>(entity, {10, 20});
    return entity;
}

engine::entitysystem::Entity createPlayer(
    engine::resourcesystem::ResourceStorage& storage,
    engine::entitysystem::ComponentManager& manager
) {
    using namespace engine::spritesystem;

    auto entity = manager.createEntity();
    manager.addComponent(
        entity,
        storage.get<LoopingAnimationData>("player-walking-south")
    );
    manager.addComponent<AnimationPlaybackData>(entity);
    return entity;
}

#endif
