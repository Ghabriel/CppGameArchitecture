#ifndef SPRITE_SYSTEM_PLAY_ANIMATION_HPP
#define SPRITE_SYSTEM_PLAY_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include "../../entity-system/include.hpp"
#include "../../utils/xtrace.hpp"
#include "AnimationData.hpp"
#include "AnimationPlaybackData.hpp"
#include "LoopingAnimationData.hpp"

namespace engine::spritesystem {
    template<typename TAnimationData>
    class AnimationPlayer {
     public:
        void setPlaybackData(AnimationPlaybackData&);
        void setAnimationData(const TAnimationData&);
        bool isFinished() const;
        void tick(double timeSinceLastFrame);
        const Frame* getCurrentFrame() const;

     private:
        AnimationPlaybackData* playbackData;
        const TAnimationData* animationData;
    };

    template<typename TAnimationData>
    void playAnimations(entitysystem::ComponentManager& manager) {
        static AnimationPlayer<TAnimationData> animationPlayer;

        manager.forEachEntity<TAnimationData, AnimationPlaybackData>(
            [&](
                entitysystem::Entity entity,
                TAnimationData& animationData,
                AnimationPlaybackData& playbackData
            ) {
                animationPlayer.setPlaybackData(playbackData);
                animationPlayer.setAnimationData(animationData);
                animationPlayer.tick(2);

                if (animationPlayer.isFinished()) {
                    manager.removeComponent<AnimationPlaybackData>(entity);
                    return;
                }

                const Frame* frame = animationPlayer.getCurrentFrame();
                animationData.sprite.setTextureRect(sf::IntRect{
                    frame->x,
                    frame->y,
                    frame->width,
                    frame->height
                });
            }
        );
    }

    template<>
    void AnimationPlayer<AnimationData>::setPlaybackData(AnimationPlaybackData& data) {
        playbackData = &data;
    }

    template<>
    void AnimationPlayer<AnimationData>::setAnimationData(const AnimationData& data) {
        animationData = &data;
    }

    template<>
    bool AnimationPlayer<AnimationData>::isFinished() const {
        return playbackData->currentFrameIndex >= animationData->frames.size();
    }

    template<>
    void AnimationPlayer<AnimationData>::tick(double timeSinceLastFrame) {
        if (isFinished()) {
            return;
        }

        auto& currentFrameIndex = playbackData->currentFrameIndex;
        playbackData->currentFrameElapsedTime += timeSinceLastFrame;
        const Frame& currentFrame = animationData->frames.at(currentFrameIndex);

        if (playbackData->currentFrameElapsedTime >= currentFrame.lengthInMilliseconds) {
            ++currentFrameIndex;
            playbackData->currentFrameElapsedTime -= currentFrame.lengthInMilliseconds;
        }
    }

    template<>
    const Frame* AnimationPlayer<AnimationData>::getCurrentFrame() const {
        if (isFinished()) {
            return nullptr;
        }

        return &animationData->frames.at(playbackData->currentFrameIndex);
    }

    template<>
    void AnimationPlayer<LoopingAnimationData>::setPlaybackData(
        AnimationPlaybackData& data
    ) {
        playbackData = &data;
    }

    template<>
    void AnimationPlayer<LoopingAnimationData>::setAnimationData(
        const LoopingAnimationData& data
    ) {
        animationData = &data;
    }

    template<>
    bool AnimationPlayer<LoopingAnimationData>::isFinished() const {
        return false;
    }

    template<>
    void AnimationPlayer<LoopingAnimationData>::tick(double timeSinceLastFrame) {
        auto& currentFrameIndex = playbackData->currentFrameIndex;
        playbackData->currentFrameElapsedTime += timeSinceLastFrame;
        const Frame& currentFrame = animationData->frames.at(currentFrameIndex);

        if (playbackData->currentFrameElapsedTime >= currentFrame.lengthInMilliseconds) {
            currentFrameIndex = (currentFrameIndex + 1) % animationData->frames.size();
            playbackData->currentFrameElapsedTime -= currentFrame.lengthInMilliseconds;
        }
    }

    template<>
    const Frame* AnimationPlayer<LoopingAnimationData>::getCurrentFrame() const {
        return &animationData->frames.at(playbackData->currentFrameIndex);
    }
}

#endif