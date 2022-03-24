#include <math.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <complex>
#include <iostream>
#include <vector>
#define WINSIZE 1000
#define MAX_ITERATIONS 255

static sf::Color map[16] = {
    sf::Color(66, 30, 15),
    sf::Color(25, 7, 26),
    sf::Color(9, 1, 47),
    sf::Color(4, 4, 73),
    sf::Color(0, 7, 100),
    sf::Color(12, 44, 138),
    sf::Color(24, 82, 177),
    sf::Color(57, 125, 209),
    sf::Color(134, 181, 229),
    sf::Color(211, 236, 248),
    sf::Color(241, 233, 191),
    sf::Color(248, 201, 95),
    sf::Color(255, 170, 0),
    sf::Color(204, 128, 0),
    sf::Color(153, 87, 0),
    sf::Color(106, 52, 3)};

using Complex = std::complex<double>;

/*
    iterate through every point in the window.
    Map that point to a point on the complex plane.
    Decide whether it is in the Mendelbrot set.
    M = set of complex numbers { c : distance of f_c is bounded. f_c = f_c(z) = z^2 - c }
    Based on the number of iterations taken,
    map it to a color (i used a cringe palette)
    TODO: implement threading, add zoom capabilities
*/

static const sf::Color Colorize(const int iterations) {
    unsigned int mapping = iterations % 16;
    return map[mapping];
}

// maps a coordinate in window to a point on our complex plane
const Complex pixel_to_point(unsigned int x, unsigned int y, const Complex& bounds) {
    double real = (x - 0.5 * WINSIZE) /
                  ((double)WINSIZE / (2.0 * bounds.real()));
    double i = (0.5 * WINSIZE - (double)y) / (WINSIZE / (2.0 * bounds.imag()));
    return Complex(real, i);
}

// should really spit this up
void iterate(std::array<std::array<sf::Color, WINSIZE>, WINSIZE>& pixels,
             const Complex& bounds, const Complex& origin) {
    // here goes nutin'
    unsigned int iterations = 0;
    for (int i = 0; i < pixels.size(); i++) {
        for (int j = 0; j < pixels[0].size(); j++) {
            Complex coord = (pixel_to_point(j, i, bounds) + origin);
            for (Complex z = 0; std::norm(z) <= 4.f && iterations <= MAX_ITERATIONS; iterations++) {
                z = pow(z, 2) + coord;
            }
            pixels[i][j] = Colorize(iterations);
            iterations = 0;
        }
    }
}

int main() {
    Complex origin(0, 0);
    Complex bounds(2, 2);  // x axis x:-2.5 -> 2.5, y: -2.5 -> 2.5
    const int length = WINSIZE;
    sf::Thread thread();
    bool should_update = true;
    // initialize pixels and color a plane
    auto pixels = std::array<std::array<sf::Color, length>, length>();
    sf::Texture texture;
    sf::Image image;

    // window init
    sf::RenderWindow window(sf::VideoMode(length, length), "mendelbrot");
    window.setFramerateLimit(300);
    window.setKeyRepeatEnabled(false);
    window.display();
    while (window.isOpen()) {
        // event loop
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
            else if (e.type == sf::Event::KeyPressed) {
                switch (e.key.code) {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::Left:
                        origin = {origin.real() - (bounds.real() / 10), origin.imag()};
                        //  bounds = Complex(bounds.real() - 0.1, bounds.imag());
                        break;
                }

                should_update = true;
            } else if (e.type == sf::Event::MouseWheelMoved) {
                bounds = {bounds.real() - bounds.real() / 25,
                          bounds.imag() - bounds.imag() / 25};
                should_update = true;
            }
        }

        // display mouse info so long as mouse in window
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        sf::FloatRect window_bounds(sf::Vector2f(0, 0), window.getDefaultView().getSize());

        if (window_bounds.contains((sf::Vector2f)mouse_pos)) {
            // std::cout << "mouse x:" << mouse_pos.x << "\tmouse y:" << mouse_pos.y << "\n";
            // Complex complex_coord = pixel_to_point(mouse_pos.x, mouse_pos.y, bounds);
            // std::cout << "complex real: " << complex_coord.real() << " imag: " << complex_coord.imag() << "\n";
            ;
            // circle.setPosition((sf::Vector2f)mouse_pos);
        }

        //  render portion
        window.clear();
        if (should_update) {
            should_update = false;
            iterate(pixels, bounds, origin);
            std::cout << "new origin:" << origin << "\n";
            std::cout << "new bounds:" << bounds << "\n";
        }
        image.create(length, length, (sf::Uint8*)pixels.data());
        texture.loadFromImage(image);
        texture.loadFromImage(image);

        window.draw(sf::Sprite(texture));
        window.display();
    }
}