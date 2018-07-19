#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

#include <atomic>
#include <functional>
#include <thread>
#include "../utils/Clock.hpp"

namespace engine::gameloop {
    class GameLoop {
        using Clock = utils::Clock;
     public:
        GameLoop(
            std::function<void(GameLoop&)> update,
            std::function<void(GameLoop&, double)> render
        );

        GameLoop(GameLoop&) = delete;
        GameLoop(GameLoop&&) = delete;
        GameLoop& operator=(GameLoop&) = delete;
        GameLoop& operator=(GameLoop&&) = delete;

        void setUpdateFrequency(int ticksPerSecond);

        void start();
        void stop();
        void join();

     private:
        std::function<void(GameLoop&)> update;
        std::function<void(GameLoop&, double)> render;
        int updatePeriod;

        std::thread updateThread;
        std::thread renderThread;
        bool running = false;
        Clock clock;
        std::atomic<intmax_t> nextUpdate;

        void spawnUpdateThread();
        void spawnRenderThread();
    };
}

#endif
