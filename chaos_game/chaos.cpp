#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <complex>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
// settings
static const int win_size = 1250;
static const int num_dots = 3;
static const float dot_radius = 10;
static const float travel_distance = 2.f;
void add_dot(int& iteration,
             const std::vector<sf::CircleShape>& initial_dots, std::vector<sf::CircleShape>& dots) {
    sf::CircleShape new_circle(dots[dots.size() - 1]);
    const int index = std::rand() % num_dots;
    std::cout << new_circle.getPosition().x << " " << new_circle.getPosition().y << "\n";
    sf::Vector2f delta = dots[dots.size() - 1].getPosition() + initial_dots[index].getPosition();
    delta /= travel_distance;
    // int half_distance = sqrt((pow(delta.x, 2) + pow(delta.y, 2))) / 2;
    new_circle.setPosition(delta);
    dots.push_back(new_circle);
    iteration++;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(win_size, win_size), "Chaos Game Simulation");

    // initial state
    std::vector<sf::CircleShape> initial_dots;
    std::vector<sf::CircleShape> all_dots;
    int iteration_count = 0;
    bool should_iterate = false;

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/UbuntuMono-R.ttf"))
        std::cout << "ERR: no such font\n";
    window.setFramerateLimit(144);
    window.setKeyRepeatEnabled(false);
    window.display();
    while (window.isOpen()) {
        sf::Event event;

        // event loop
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    else if (event.key.code == sf::Keyboard::Equal)
                        should_iterate = !should_iterate;
                    break;
                case sf::Event::MouseButtonPressed:
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    if (initial_dots.size() < num_dots) {
                        initial_dots.push_back(sf::CircleShape(dot_radius, 100));
                        auto* circle = &initial_dots[initial_dots.size() - 1];
                        circle->setOrigin(circle->getRadius(), circle->getRadius());
                        circle->setFillColor(sf::Color::Green);
                        circle->setPosition((sf::Vector2f)mouse_pos);
                    } else if (all_dots.size() == 0) {
                        all_dots.push_back(sf::CircleShape(dot_radius / 2, 100));
                        all_dots[0].setOrigin(all_dots[0].getRadius(), all_dots[0].getRadius());
                        all_dots[0].setPosition((sf::Vector2f)mouse_pos);
                        all_dots[0].setFillColor(sf::Color::White);
                    }
                    break;
            }
        }

        window.clear(sf::Color(15, 15, 15));

        // draw initial dots with labels
        for (auto i = 0; i < initial_dots.size(); i++) {
            char letter = 'a' + i;
            sf::Text label(letter, font);
            sf::Vector2f offset(initial_dots[i].getRadius(), initial_dots[i].getRadius());
            label.setPosition(initial_dots[i].getPosition() + offset);
            window.draw(label);
            window.draw(initial_dots[i]);
        }

        // draw iteration cont
        std::string iter_str = "iterations: i = " + std::to_string(iteration_count);
        sf::Text iteration_display(iter_str, font);
        window.draw(iteration_display);

        // display info regarding state (looks like code vomit sowwy)
        std::string state_info;
        if (initial_dots.size() < num_dots) {
            state_info = "Click anywhere to draw initial points";
        } else if (all_dots.size() == 0)
            state_info = "Set initial tracepoint";
        else
            state_info = "(s)tart/stop iteration\t(c)lear\t(+,-)speed up/slow down";
        sf::Text state_display(state_info, font);
        auto state_bounds = state_display.getGlobalBounds();

        state_display.setPosition(window.getSize().x -
                                      state_bounds.width - state_bounds.left - 25,
                                  window.getSize().y -
                                      state_bounds.height - state_bounds.top - 25);
        window.draw(state_display);

        if (all_dots.size()) {
            if (should_iterate)
                add_dot(iteration_count, initial_dots, all_dots);

            for (auto const& dot : all_dots)
                window.draw(dot);

            sf::CircleShape copy(all_dots[all_dots.size() - 1]);
            copy.setOutlineColor(sf::Color::Black);
            copy.setOutlineThickness(2);
            // display tracepoint text
            sf::Text tp("tracepoint", font, 15);
            tp.setPosition(copy.getPosition() + sf::Vector2f(copy.getRadius(), copy.getRadius()));
            window.draw(tp);
            window.draw(copy);
        }

        window.display();
    }
}