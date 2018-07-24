#ifndef SPRITE_SYSTEM_ANIMATION_PLAYBACK_DATA_HPP
#define SPRITE_SYSTEM_ANIMATION_PLAYBACK_DATA_HPP

namespace engine::spritesystem {
    struct AnimationPlaybackData {
        size_t currentFrameIndex = 0;
        double currentFrameElapsedTime = 0;
    };
}

#endif
