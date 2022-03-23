#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>

int main() {
    // init window
    sf::RenderWindow window(sf::VideoMode(800, 800), "sfml-app");
    window.setFramerateLimit(144);
    window.setKeyRepeatEnabled(false);

    // get fonts
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/UbuntuMono-R.ttf"))
        std::cout << "ERR: no such font\n";

    // this many lines of code to set text??!??!?!
    sf::Text text;
    text.setFont(font);
    text.setString("(Q)uit\t(F)aster\t(S)lower (I)nitiate spin/stop\nLeft click To change direction");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // get/load image into a 500x500 texture
    sf::Texture t1;
    if (!t1.loadFromFile("assets/img/cringe.jpeg", sf::IntRect(0, 0, 500, 500)))
        std::cout << "improper load from file\n";
    t1.setSmooth(true);

    // put the texture on this sprite, center
    sf::Sprite sprite(t1);
    auto win_size = window.getSize();
    sprite.setOrigin((sf::Vector2f)t1.getSize() / 2.f);
    sprite.setPosition(win_size.x / 2, win_size.y / 2);

    // initial movement
    sf::Vector2f delta(-1.5, 1.5);

    // for rotation
    float rot_direction = 1.f;
    float rot_magnitude = 2.f;

    //  main loop
    sf::Clock clock;
    sf::Time total_elapsed;
    bool should_spin = false;
    while (window.isOpen()) {
        // check/handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::MouseButtonPressed) {
                rot_direction *= -1;  // make cringe guy spin in opposite direction
                auto mousePos = sf::Mouse::getPosition(window);
                std::cout << "MOUSE CLICK AT " << mousePos.x
                          << "y: " << mousePos.y << std::endl;

            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    std::cout << "Quitting...\n";
                    sf::sleep(sf::seconds(0.4));
                    window.close();
                } else if (event.key.code == sf::Keyboard::S)
                    rot_magnitude /= 1.5;
                else if (event.key.code == sf::Keyboard::F)
                    rot_magnitude *= 1.5;
                else if (event.key.code == sf::Keyboard::I)
                    should_spin = !should_spin;
            }
        }
        // debugging / playing around
        sf::Time curr_time = clock.getElapsedTime();
        if (curr_time.asSeconds() > 10) {
            total_elapsed += curr_time;
            std::cout << "Time elapsed: " << total_elapsed.asSeconds() << "s\n";
            clock.restart();
        }

        window.clear();

        // physics

        //  rotation
        if (should_spin) sprite.rotate(rot_magnitude * rot_direction);

        // translation
        sf::FloatRect sprite_rect = sprite.getGlobalBounds();
        auto curr_pos = sprite.getPosition();
        auto window_dim = sf::Vector2f(window.getSize());

        std::cout << "rect top: " << sprite_rect.top << "\n";
        std::cout << "window y: " << window_dim.y << "\n";

        // left
        if (sprite_rect.left < 0)
            delta.x *= -1;
        // right
        else if (sprite_rect.left + sprite_rect.width > window_dim.x)
            delta.x *= -1;
        // bottom
        if (sprite_rect.top + sprite_rect.height > window_dim.y)
            delta.y *= -1;
        // top
        else if (sprite_rect.top < 0) {
            delta.y *= -1;
        }
        sprite.setPosition(curr_pos + delta);

        // down

        // draw the cringe sprite to screen
        window.draw(sprite);
        window.draw(text);
        window.display();
    }
}