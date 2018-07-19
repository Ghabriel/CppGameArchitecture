#include "engine/game-loop/GameLoop.hpp"

using namespace engine::gameloop;

GameLoop::GameLoop(
    std::function<void(GameLoop&)> update,
    std::function<void(GameLoop&, double)> render
) : update(update), render(render) {
    setUpdateFrequency(25);
}

void GameLoop::setUpdateFrequency(int ticksPerSecond) {
    updatePeriod = 1000 / ticksPerSecond;
}

void GameLoop::start() {
    running = true;
    clock.restart();
    spawnUpdateThread();
    spawnRenderThread();
}

void GameLoop::stop() {
    running = false;
}

void GameLoop::join() {
    updateThread.join();
    renderThread.join();
}

void GameLoop::spawnUpdateThread() {
    updateThread = std::thread([&]() {
        nextUpdate = clock.getTickCount();

        while (running) {
            auto now = clock.getTickCount();

            if (now >= nextUpdate) {
                update(*this);
                nextUpdate += updatePeriod;
            }
        }
    });
}

void GameLoop::spawnRenderThread() {
    renderThread = std::thread([&]() {
        while (running) {
            auto now = clock.getTickCount();
            double timeSinceUpdate = now + updatePeriod - nextUpdate;

            if (timeSinceUpdate > updatePeriod) {
                // Update is being processed, assume that it already happened
                timeSinceUpdate -= updatePeriod;
            }

            double interpolation = timeSinceUpdate / updatePeriod;
            render(*this, interpolation);
        }
    });
}
