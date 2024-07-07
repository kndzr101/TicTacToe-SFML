#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(sf::Texture& normalTexture, sf::Texture& xTexture, sf::Texture& oTexture, sf::Vector2f position);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    bool isClicked() const;
    void setPlayer(int player);
    void resetState();

private:
    enum State { Normal, X, O } state;

    sf::Sprite normalSprite;
    sf::Sprite xSprite;
    sf::Sprite oSprite;
    sf::Sprite* currentSprite;

    bool clicked;
    int currentPlayer;
};

Button::Button(sf::Texture& normalTexture, sf::Texture& xTexture, sf::Texture& oTexture, sf::Vector2f position)
    : state(Normal), clicked(false), currentPlayer(0) {
    normalSprite.setTexture(normalTexture);
    xSprite.setTexture(xTexture);
    xSprite.setScale(0.5, 0.5);

    oSprite.setTexture(oTexture);
    oSprite.setScale(0.25, 0.325);

    normalSprite.setPosition(position);
    xSprite.setPosition(position);
    oSprite.setPosition(position);

    currentSprite = &normalSprite;
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool mouseOver = currentSprite->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));

    if (mouseOver && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        clicked = true;
	if(state == Normal){
		if (currentPlayer == 1) {
		    state = X;
		    currentSprite = &xSprite;
		} else if (currentPlayer == 2) {
		    state = O;
		    currentSprite = &oSprite;
		}
	}
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(*currentSprite);
}

bool Button::isClicked() const {
    return clicked;
}

void Button::setPlayer(int player) {
    currentPlayer = player;
}
void Button::resetState(){
	currentPlayer = 1;
	state = Normal;
	currentSprite = &normalSprite;
}

