#ifndef LOAD_RESOURCES_HPP
#define LOAD_RESOURCES_HPP

#include <cassert>
#include <SFML/Graphics.hpp>
#include "engine/resource-system/ResourceStorage.hpp"

void loadResources(engine::resourcesystem::ResourceStorage& storage) {
    sf::Texture texture;
    assert(texture.loadFromFile("resources/sprite-test.png"));
    storage.store("player-sprite", texture);
}

#endif
