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
static const sf::Color BG_COLOR(255, 255, 255);
static const sf::Color INITIAL_POINTS_COLOR(sf::Color::Green);
static const sf::Color DOT_COLOR(sf::Color::Black);
static const int win_size = 1250;
static const int num_dots = 5;
static const float dot_radius = 1;
static const float travel_distance = 2.f;
static int prev_point = -1;  // for squares/other rules to apply

void add_dot(int& iteration,
             const std::vector<sf::CircleShape>& initial_dots, std::vector<sf::CircleShape>& dots) {
    sf::CircleShape new_circle(dots[dots.size() - 1]);

    int index;
    // A rule to make 4/5 points a fractal. Otherwise with the default chaos game rule, nothing happens.
    if (num_dots > 3) {
        while ((index = rand() % num_dots) == prev_point)
            ;
        prev_point = index;
    }
    // else if(num_dots == )
    sf::Vector2f delta = dots[dots.size() - 1].getPosition() + initial_dots[index].getPosition();
    delta.x = abs(delta.x);
    delta.y = abs(delta.y);
    delta /= 2.f;
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
                    else if (event.key.code == sf::Keyboard::C) {
                        should_iterate = false;
                        initial_dots.clear();
                        all_dots.clear();
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    if (initial_dots.size() < num_dots) {
                        initial_dots.push_back(sf::CircleShape(dot_radius, 100));
                        auto* circle = &initial_dots[initial_dots.size() - 1];
                        circle->setOrigin(circle->getRadius(), circle->getRadius());
                        circle->setFillColor(INITIAL_POINTS_COLOR);
                        circle->setPosition((sf::Vector2f)mouse_pos);
                    } else if (all_dots.size() == 0) {
                        all_dots.push_back(sf::CircleShape(dot_radius / 2, 100));
                        all_dots[0].setOrigin(all_dots[0].getRadius(), all_dots[0].getRadius());
                        all_dots[0].setPosition((sf::Vector2f)mouse_pos);
                        all_dots[0].setFillColor(DOT_COLOR);
                    }
                    break;
            }
        }

        window.clear(BG_COLOR);

        // draw initial dots with labels
        for (auto i = 0; i < initial_dots.size(); i++) {
            char letter = 'a' + i;
            sf::Text label(letter, font);
            sf::Vector2f offset(initial_dots[i].getRadius(), initial_dots[i].getRadius());
            label.setPosition(initial_dots[i].getPosition() + offset);
            label.setFillColor(sf::Color(BG_COLOR.r * -1, BG_COLOR.g * -1, BG_COLOR.b * -1));
            window.draw(label);
            window.draw(initial_dots[i]);
        }

        // draw iteration cont
        std::string iter_str = "iterations: i = " + std::to_string(iteration_count);
        sf::Text iteration_display(iter_str, font);
        iteration_display.setFillColor(sf::Color(BG_COLOR.r * -1, BG_COLOR.g * -1, BG_COLOR.b * -1));
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
        state_display.setFillColor(sf::Color(BG_COLOR.r * -1, BG_COLOR.g * -1, BG_COLOR.b * -1));
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
            copy.setOutlineColor(sf::Color::Red);
            copy.setOutlineThickness(2);
            // display tracepoint text
            sf::Text tp("tracepoint", font, 15);
            tp.setPosition(copy.getPosition() + sf::Vector2f(copy.getRadius(), copy.getRadius()));
            tp.setFillColor(sf::Color(BG_COLOR.r * -1, BG_COLOR.g * -1, BG_COLOR.b * -1));
            window.draw(tp);
            window.draw(copy);
        }

        window.display();
    }
}