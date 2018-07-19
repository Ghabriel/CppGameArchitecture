#ifndef UTILS_CLOCK_HPP
#define UTILS_CLOCK_HPP

#include <chrono>

namespace engine::utils {
    class Clock {
     public:
        using InternalClock = std::chrono::high_resolution_clock;
        using TimePoint = typename InternalClock::time_point;

        // Returns the number of milliseconds elapsed since startTime.
        intmax_t getTickCount() const {
            using namespace std::chrono;
            return duration_cast<milliseconds>(now() - startTime).count();
        }

        // Returns the current time point, according to the internal clock.
        TimePoint now() const {
            return InternalClock::now();
        }

        // Allows clock re-use.
        void restart() {
            startTime = now();
        }

     private:
        TimePoint startTime = now();
    };
}

#endif
