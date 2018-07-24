#ifndef SPRITE_SYSTEM_ANIMATION_DATA_HPP
#define SPRITE_SYSTEM_ANIMATION_DATA_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Frame.hpp"

namespace engine::spritesystem {
    struct AnimationData {
        sf::Sprite sprite;
        std::vector<Frame> frames;
    };
}

#endif
