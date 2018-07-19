#ifndef RENDERER_SYSTEM_HPP
#define RENDERER_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include "components/CircularTraits.hpp"
#include "components/Position.hpp"
#include "engine/entity-system/include.hpp"

inline void render(
    sf::RenderWindow& window,
    engine::entitysystem::ComponentManager& manager
) {
    using Entity = engine::entitysystem::Entity;

    manager.forEachEntity<CircularTraits, Position>(
        [&](Entity entity, CircularTraits& traits, Position& position) {
            sf::CircleShape shape(traits.radius);
            shape.setPosition(position.x, position.y);
            shape.setFillColor(traits.fillColor);
            window.draw(shape);
        }
    );
}

#endif
