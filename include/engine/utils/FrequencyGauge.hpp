#ifndef UTILS_FREQUENCY_GAUGE_HPP
#define UTILS_FREQUENCY_GAUGE_HPP

#include "Clock.hpp"

namespace engine::utils {
    /**
     * Simplifies frequency-related calculations.
     * Can be used e.g to measure the FPS count of the game.
     */
    class FrequencyGauge {
    public:
        void tick() {
            ++counter;
        }

        intmax_t tickCount() const {
            return counter;
        }

        long double measure() const {
            return counter * 1000.0L / clock.getTickCount();
        }

    private:
        Clock clock;
        intmax_t counter = 0;
    };
}

#endif
