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
    manager.addComponent<CircularTraits>(entity);
    manager.getData<CircularTraits>(entity) = {100, sf::Color::Blue};
    manager.addComponent<Position>(entity);
    manager.getData<Position>(entity) = {10, 20};
    return entity;
}

engine::entitysystem::Entity createPlayer(
    engine::resourcesystem::ResourceStorage& storage,
    engine::entitysystem::ComponentManager& manager
) {
    using namespace engine::spritesystem;

    sf::Sprite sprite(storage.get<sf::Texture>("player-sprite"));

    auto entity = manager.createEntity();
    manager.addComponent<LoopingAnimationData>(entity);
    manager.getData<LoopingAnimationData>(entity) = {
        sprite,
        {
            {0, 0, 32, 48, 10},
            {32, 0, 32, 48, 10},
            {64, 0, 32, 48, 10},
            {96, 0, 32, 48, 10},
        }
    };
    manager.addComponent<AnimationPlaybackData>(entity);
    return entity;
}

#endif
