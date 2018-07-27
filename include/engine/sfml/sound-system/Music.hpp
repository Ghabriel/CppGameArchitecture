#ifndef SOUND_SYSTEM_MUSIC_HPP
#define SOUND_SYSTEM_MUSIC_HPP

#include <memory>
#include <SFML/Audio.hpp>

namespace engine::soundsystem {
    /**
     * \brief A simple wrapper around sf::Music to enable move semantics.
     * Hopefully SFML 3 will solve this problem.
     */
    class Music {
     public:
        Music();

        sf::Music& get();
        void play();
        void pause();
        void stop();

     private:
        std::unique_ptr<sf::Music> resource;
    };

    inline Music::Music() : resource(std::make_unique<sf::Music>()) { }

    inline sf::Music& Music::get() {
        return *resource;
    }

    inline void Music::play() {
        resource->play();
    }

    inline void Music::pause() {
        resource->pause();
    }

    inline void Music::stop() {
        resource->stop();
    }
}

#endif
