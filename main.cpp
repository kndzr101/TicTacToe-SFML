#include <SFML/Graphics.hpp>
#include <iostream>
#include "button.hpp"
#include "playAgainButton.hpp"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const char* GRID_PATH = "images/grid.png";
const char* BUTTON_NORMAL_PATH = "images/button_normal.png";
const char* X_PATH = "images/x-image.png";
const char* O_PATH = "images/o-image.png";
const char* FONT_PATH = "fonts/Roboto-Regular.ttf";
const char* GAME_OVER_PATH = "images/game_over.png";
const char* PLAY_AGAIN_PATH = "images/play-again.png";

enum   cellState {
    nothing = 0,
    circle = 1,
    cross = 2
};

sf::Sprite getItToSprite(sf::Texture& texture, const char* imagePath) {
    sf::Image image;
    if (!image.loadFromFile(imagePath)) {
        std::cerr << "error loading image " << imagePath << std::endl;
    }
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    return sprite;
}

bool checkWinner(cellState gameState[][3], cellState player, int x, int y) {
    int col = 0, row = 0, diag = 0, rdiag = 0;

    for (int i = 0; i < 3; ++i) {
        if (gameState[x][i] == player) col++;
        if (gameState[i][y] == player) row++;
        if (gameState[i][i] == player) diag++;
        if (gameState[i][2 - i] == player) rdiag++;
    }

    return (col == 3 || row == 3 || diag == 3 || rdiag == 3);
}

void resetGame(cellState gameState[][3], Button* buttons[][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameState[i][j] = nothing;
            buttons[i][j]->resetState();
        }
    }
}
void printGameState(cellState gameState[][3]){

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
		switch ( gameState[i][j]){
			case nothing:
				std::cout << 0 <<" ";
				break;
			case circle:
				std::cout << 1 << " ";
				break;
			case cross:
				std::cout << 2 << " " ;
				break;

		}

        }
	std::cout << std::endl;
    }
}

int main() {
    bool startAgain = true;

    while (startAgain) {
        bool gameIsOver = false;

        cellState gameState[3][3] = { {nothing, nothing, nothing}, 
                                      {nothing, nothing, nothing}, 
                                      {nothing, nothing, nothing} };

        sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "TicTacToe");

        sf::Sprite spriteGrid, gameOverSprite, playAgainSprite;
        sf::Texture gridTexture, buttonNormalTexture, xTexture, oTexture, gameOverTexture, playAgainTexture;
        spriteGrid = getItToSprite(gridTexture, GRID_PATH);
        spriteGrid.setScale(5, 5);
        spriteGrid.setPosition((SCREEN_WIDTH - spriteGrid.getGlobalBounds().width) / 2, (SCREEN_HEIGHT - spriteGrid.getGlobalBounds().height) / 2);
        gameOverSprite = getItToSprite(gameOverTexture, GAME_OVER_PATH);
        gameOverSprite.setPosition(55.f, -300.f);
        gameOverSprite.setScale(2.5f, 2.5f);
        playAgainSprite = getItToSprite(playAgainTexture, PLAY_AGAIN_PATH);
        playAgainSprite.setScale(0.5f, 0.5f);

        if (!buttonNormalTexture.loadFromFile(BUTTON_NORMAL_PATH) ||
            !xTexture.loadFromFile(X_PATH) ||
            !oTexture.loadFromFile(O_PATH) ||
            !playAgainTexture.loadFromFile(PLAY_AGAIN_PATH)) {
            std::cerr << "Error loading images" << std::endl;
            return -1;
        }

        sf::Font font;
        if (!font.loadFromFile(FONT_PATH)) {
            std::cerr << "Error loading font" << std::endl;
            return -1;
        }

        Button* buttons[3][3];
        float buttonSize = buttonNormalTexture.getSize().x * 1.315f;
        float startX = spriteGrid.getPosition().x + 200;
        float startY = spriteGrid.getPosition().y + 100;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                buttons[i][j] = new Button(buttonNormalTexture, xTexture, oTexture, sf::Vector2f(startX + j * buttonSize, startY + i * buttonSize));
            }
        }

        PlayAgainButton playAgainButton(playAgainTexture, sf::Vector2f((SCREEN_WIDTH - playAgainSprite.getGlobalBounds().width) / 2, 
                                                                       (SCREEN_HEIGHT - 2 * playAgainSprite.getGlobalBounds().height)));

        int currentPlayer = 1;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    startAgain = false;
                    window.close();
                }

                if (!gameIsOver) {
                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            buttons[i][j]->setPlayer(currentPlayer);
                            buttons[i][j]->handleEvent(event, window);
                            if (buttons[i][j]->isClicked() && gameState[i][j] == nothing) {
                                cellState currState = (currentPlayer == 1) ? cross : circle;
                                gameState[i][j] = currState;
                                if (checkWinner(gameState, currState, i, j)) {
                                    gameIsOver = true;

				    resetGame(gameState, buttons);
				    std::cout << "Game is over. Winner is player " << currentPlayer << std::endl;
                                }
                                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                            }
                        }
                    }
                } else {
                    playAgainButton.handleEvent(event, window);

                    if (playAgainButton.isClicked()) {
                        resetGame(gameState, buttons);
			printGameState(gameState);
			
                        gameIsOver = false;
			startAgain = true;
                        currentPlayer = 1;
                        playAgainButton.reset();
                        window.clear();
                    }
                }
            }

            window.clear(sf::Color(50, 50, 50, 255));
            window.draw(spriteGrid);

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    buttons[i][j]->draw(window);
                }
            }

            if (gameIsOver ) {
                window.draw(gameOverSprite);
                playAgainButton.draw(window);
            }

            window.display();
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                delete buttons[i][j];
            }
        }
    }

    return 0;
}

