#include "load-resources.hpp"

#include <cassert>
#include <SFML/Graphics.hpp>
#include "engine/sfml/sprite-system/include.hpp"

using engine::resourcesystem::ResourceStorage;

void loadTextures(ResourceStorage& storage) {
    sf::Texture texture;
    assert(texture.loadFromFile("resources/sprite-test.png"));
    storage.store("player-sprite", texture);
}

void loadAnimationData(ResourceStorage& storage) {
    using namespace engine::spritesystem;

    sf::Sprite sprite(storage.get<sf::Texture>("player-sprite"));

    LoopingAnimationData playerWalkingSouth {
        sprite,
        {
            {0, 0, 32, 48, 10},
            {32, 0, 32, 48, 10},
            {64, 0, 32, 48, 10},
            {96, 0, 32, 48, 10},
        }
    };

    storage.store("player-walking-south", playerWalkingSouth);
}

void loadResources(ResourceStorage& storage) {
    loadTextures(storage);
    loadAnimationData(storage);
}