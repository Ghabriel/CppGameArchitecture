#ifndef PROFILER_HPP
#define PROFILER_HPP

#include <chrono>

class Profiler {
 public:
    void start() {
        startTime = std::chrono::steady_clock::now();
    }

    void finish() {
        finishTime = std::chrono::steady_clock::now();
    }

    template<typename T>
    intmax_t measure() const {
        return std::chrono::duration_cast<T>(finishTime - startTime).count();
    }

    intmax_t measureAsSeconds() const {
        return measure<std::chrono::seconds>();
    }

    intmax_t measureAsMilliseconds() const {
        return measure<std::chrono::milliseconds>();
    }

    intmax_t measureAsMicroseconds() const {
        return measure<std::chrono::microseconds>();
    }

 private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point finishTime;
};

#endif
