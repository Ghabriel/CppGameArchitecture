#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "engine/entity-system/include.hpp"
#include "engine/game-loop/include.hpp"
#include "engine/utils/timing/print-fps.hpp"
#include "sfml-renderer-system.hpp"

class Renderer {
    using ComponentManager = engine::entitysystem::ComponentManager;
    using GameLoop = engine::gameloop::GameLoop;
  public:
    Renderer(ComponentManager& manager)
     : windowPtr(new sf::RenderWindow(sf::VideoMode(800, 600), "Test")),
       componentManager(manager) { }

    void operator()(GameLoop& game, double interpolation) {
        engine::utils::printFPS<2>("FPS", 2000);

        sf::RenderWindow& window = *windowPtr;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                game.stop();
            } else if (event.type == sf::Event::Resized) {
                sf::Vector2u windowSize = window.getSize();
                double width = windowSize.x;
                double height = windowSize.y;
                view = sf::View(sf::FloatRect(0, 0, width, height));
                window.setView(view);
            }
        }

        window.clear();
        render(window, componentManager);
        window.display();
    }

  private:
    std::shared_ptr<sf::RenderWindow> windowPtr;
    sf::View view;
    ComponentManager& componentManager;
};

#endif
