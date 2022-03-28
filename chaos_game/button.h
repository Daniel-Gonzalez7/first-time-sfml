#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable, public sf::Transformable {
   public:
    Button(std::string _text, sf::Vector2f _size, size_t charSize, sf::Color buttonColor, sf::Color textColor) {
        text.setString(_text);
        text.setCharacterSize(charSize);
        text.setColor(textColor);
        rect.setFillColor(buttonColor);
        rect.setSize(size);
    }
    void setFont(sf::Font &font) { text.setFont(font); }
    void setPosition(sf::Vector2f);
    bool isMouseOver();

   private:
    sf::RectangleShape rect;
    sf::Text text;
};
