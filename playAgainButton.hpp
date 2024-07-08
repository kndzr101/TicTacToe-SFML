#include <SFML/Graphics.hpp>
#include <string>

class PlayAgainButton {
public:
    PlayAgainButton(sf::Texture& normalTexture, sf::Vector2f position);
    void draw(sf::RenderWindow& window);
    bool isClicked() const;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void reset();  

private:
    sf::Sprite normalSprite;
    bool clicked;
};

PlayAgainButton::PlayAgainButton(sf::Texture& normalTexture, sf::Vector2f position) : clicked(false) {
    normalSprite.setTexture(normalTexture);
    normalSprite.setPosition(position);
}

void PlayAgainButton::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool mouseOver = normalSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));

    if (mouseOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        clicked = true;
    }
}

void PlayAgainButton::draw(sf::RenderWindow& window) {
    normalSprite.setScale(0.5, 0.5);
    window.draw(normalSprite);
}

bool PlayAgainButton::isClicked() const {
    return clicked;
}

void PlayAgainButton::reset() {
    clicked = false;
}

