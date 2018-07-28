#ifndef RENDERER_SYSTEM_HPP
#define RENDERER_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include "engine/entity-system/forward-declarations.hpp"
#include "engine/resource-system/forward-declarations.hpp"

void render(
    sf::RenderWindow& window,
    engine::entitysystem::ComponentManager& manager,
    engine::resourcesystem::ResourceStorage& storage
);

#endif
