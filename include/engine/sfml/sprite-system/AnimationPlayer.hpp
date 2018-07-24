#ifndef SPRITE_SYSTEM_ANIMATION_PLAYER_HPP
#define SPRITE_SYSTEM_ANIMATION_PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "AnimationData.hpp"
#include "LoopingAnimationData.hpp"

namespace engine::spritesystem {
    template<typename TAnimationData>
    class AnimationPlayer {
     public:
        AnimationPlayer(const TAnimationData&);

        bool isFinished() const;
        void tick(double timeSinceLastFrame);
        const Frame* getCurrentFrame() const;

     private:
        const TAnimationData& animationData;
        size_t currentFrameIndex = 0;
        double currentFrameElapsedTime = 0;
    };

    template<>
    AnimationPlayer<AnimationData>::AnimationPlayer(const AnimationData& data)
     : animationData(data) { }

    template<>
    bool AnimationPlayer<AnimationData>::isFinished() const {
        return currentFrameIndex >= animationData.frames.size();
    }

    template<>
    void AnimationPlayer<AnimationData>::tick(double timeSinceLastFrame) {
        if (isFinished()) {
            return;
        }

        currentFrameElapsedTime += timeSinceLastFrame;
        const Frame& currentFrame = animationData.frames.at(currentFrameIndex);

        if (currentFrameElapsedTime >= currentFrame.lengthInMilliseconds) {
            ++currentFrameIndex;
            currentFrameElapsedTime -= currentFrame.lengthInMilliseconds;
        }
    }

    template<>
    const Frame* AnimationPlayer<AnimationData>::getCurrentFrame() const {
        if (isFinished()) {
            return nullptr;
        }

        return &animationData.frames.at(currentFrameIndex);
    }

    template<>
    AnimationPlayer<LoopingAnimationData>::AnimationPlayer(const LoopingAnimationData& data)
     : animationData(data) { }

    template<>
    bool AnimationPlayer<LoopingAnimationData>::isFinished() const {
        return false;
    }

    template<>
    void AnimationPlayer<LoopingAnimationData>::tick(double timeSinceLastFrame) {
        currentFrameElapsedTime += timeSinceLastFrame;
        const Frame& currentFrame = animationData.frames.at(currentFrameIndex);

        if (currentFrameElapsedTime >= currentFrame.lengthInMilliseconds) {
            currentFrameIndex = (currentFrameIndex + 1) % animationData.frames.size();
            currentFrameElapsedTime -= currentFrame.lengthInMilliseconds;
        }
    }

    template<>
    const Frame* AnimationPlayer<LoopingAnimationData>::getCurrentFrame() const {
        return &animationData.frames.at(currentFrameIndex);
    }
}

#endif
