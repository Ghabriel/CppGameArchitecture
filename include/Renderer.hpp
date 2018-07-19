#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "engine/game-loop/include.hpp"
#include "engine/utils/print-fps.hpp"

class Renderer {
    using GameLoop = engine::gameloop::GameLoop;
 public:
    Renderer() : windowPtr(new sf::RenderWindow(sf::VideoMode(800, 600), "Test")) {}

    void operator()(GameLoop& game, double interpolation) {
        engine::utils::printFPS<2>("FPS", 2000);

        static sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

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
        window.draw(shape);
        window.display();
    }

  private:
    std::shared_ptr<sf::RenderWindow> windowPtr;
    sf::View view;
};

#endif
