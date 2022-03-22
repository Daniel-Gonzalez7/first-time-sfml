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
    text.setString("(Q)uit\t(F)aster\t(S)lower\nLeft click To change direction");
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
    sprite.setPosition(win_size.x / 2, win_size.y / 2);
    // sprite.setOrigin(win_size.x / 2, win_size.x / 2);

    //  main loop
    sf::Clock clock;
    sf::Time total_elapsed;
    float rot_direction = 1.f;
    float rot_magnitude = 2.f;
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

        // do some stupid sh** with the sprite?
        sprite.rotate(rot_magnitude * rot_direction);

        // draw the cringe sprite to screen
        window.draw(sprite);
        window.draw(text);
        window.display();
    }
}