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

struct Move {
    int x;
    int y;
};

enum cellState {
    nothing = 0,
    circle = 1,
    cross = 2
};

enum machineStates {
    PLAYER_ONE_TURN = 0,
    PLAYER_TWO_TURN = 1,
    END_GAME_SCREEN = 2,
    CLOSE_WINDOW = 3
};

sf::Sprite getItToSprite(sf::Texture& texture, const char* imagePath) {
    if (!texture.loadFromFile(imagePath)) {
        std::cerr << "Error loading image " << imagePath << std::endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    return sprite;
}

bool isMovesLeft(cellState gameState[][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameState[i][j] == nothing) {
                return true;
            }
        }
    }
    return false;
}

int evaluate(cellState gameState[][3]) {
    for (int row = 0; row < 3; ++row) {
        if (gameState[row][0] == gameState[row][1] && gameState[row][1] == gameState[row][2]) {
            if (gameState[row][0] == circle) {
                return +10;
            } else if (gameState[row][0] == cross) {
                return -10;
            }
        }
    }
    for (int col = 0; col < 3; ++col) {
        if (gameState[0][col] == gameState[1][col] && gameState[1][col] == gameState[2][col]) {
            if (gameState[0][col] == circle) {
                return +10;
            } else if (gameState[0][col] == cross) {
                return -10;
            }
        }
    }
    if (gameState[0][0] == gameState[1][1] && gameState[1][1] == gameState[2][2]) {
        if (gameState[0][0] == circle) {
            return +10;
        } else if (gameState[0][0] == cross) {
            return -10;
        }
    }
    if (gameState[0][2] == gameState[1][1] && gameState[1][1] == gameState[2][0]) {
        if (gameState[0][2] == circle) {
            return +10;
        } else if (gameState[0][2] == cross) {
            return -10;
        }
    }
    return 0;
}

int minimax(cellState gameState[][3], int depth, bool isMax) {
    int score = evaluate(gameState);

    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (!isMovesLeft(gameState)) return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (gameState[i][j] == nothing) {
                    gameState[i][j] = circle;
                    best = std::max(best, minimax(gameState, depth + 1, !isMax));
                    gameState[i][j] = nothing;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (gameState[i][j] == nothing) {
                    gameState[i][j] = cross;
                    best = std::min(best, minimax(gameState, depth + 1, !isMax));
                    gameState[i][j] = nothing;
                }
            }
        }
        return best;
    }
}

Move findBestMove(cellState gameState[][3]) {
    int bestVal = -1000;
    Move bestMove;
    bestMove.x = -1;
    bestMove.y = -1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameState[i][j] == nothing) {
                gameState[i][j] = circle;
                int moveVal = minimax(gameState, 0, false);
                gameState[i][j] = nothing;
                if (moveVal > bestVal) {
                    bestMove.x = i;
                    bestMove.y = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

bool checkWinner(cellState gameState[][3], cellState player, int x, int y) {
    int col = 0, row = 0, diag = 0, rdiag = 0;
    bool draw = true;

    for (int i = 0; i < 3; ++i) {
        if (gameState[x][i] == player) col++;
        if (gameState[i][y] == player) row++;
        if (gameState[i][i] == player) diag++;
        if (gameState[i][2 - i] == player) rdiag++;
    }

    if (col == 3 || row == 3 || diag == 3 || rdiag == 3) {
        return true;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (gameState[i][j] == nothing) {
                draw = false;
                break;
            }
        }
        if (!draw) break;
    }

    return draw;
}

void resetGame(cellState gameState[][3], Button* buttons[][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            gameState[i][j] = nothing;
            buttons[i][j]->resetState();
        }
    }
}

void printGameState(cellState gameState[][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            switch (gameState[i][j]) {
                case nothing:
                    std::cout << 0 << " ";
                    break;
                case circle:
                    std::cout << 1 << " ";
                    break;
                case cross:
                    std::cout << 2 << " ";
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
        startAgain = false;

        cellState gameState[3][3] = {
            {nothing, nothing, nothing},
            {nothing, nothing, nothing},
            {nothing, nothing, nothing}
        };

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
        machineStates currGameState = PLAYER_ONE_TURN;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                if (currGameState == PLAYER_ONE_TURN) {
                    for (int i = 0; i < 3; ++i) {
                        for (int j = 0; j < 3; ++j) {
                            buttons[i][j]->setPlayer(currentPlayer);
                            buttons[i][j]->handleEvent(event, window);
                            if (buttons[i][j]->isClicked() && gameState[i][j] == nothing) {
                                cellState currState = cross;
                                gameState[i][j] = currState;
                                if (checkWinner(gameState, currState, i, j)) {
                                    gameIsOver = true;
                                    currGameState = END_GAME_SCREEN;
                                } else {
                                    currentPlayer = 2;
                                    currGameState = PLAYER_TWO_TURN;
                                }
                            }
                        }
                    }
                } else if (currGameState == PLAYER_TWO_TURN) {
			std::cout << "we are entering second player"<<std::endl;
                    Move bestMove = findBestMove(gameState);
                    if (bestMove.x != -1 && bestMove.y != -1) {
                        buttons[bestMove.x][bestMove.y]->setPlayer(currentPlayer);
			buttons[bestMove.x][bestMove.y]->setSprite(2);
                        cellState currState = circle;
                        gameState[bestMove.x][bestMove.y] = currState;
                        if (checkWinner(gameState, currState, bestMove.x, bestMove.y)) {
                            gameIsOver = true;
                            currGameState = END_GAME_SCREEN;
                        } else {
                            currentPlayer = 1;
                            currGameState = PLAYER_ONE_TURN;
                        }
                    }
                } else if (currGameState == END_GAME_SCREEN) {
                    playAgainButton.handleEvent(event, window);

                    if (playAgainButton.isClicked()) {
                        resetGame(gameState, buttons);
                        printGameState(gameState);
                        gameIsOver = false;
                        startAgain = true;
                        currentPlayer = 1;
                        playAgainButton.reset();
                        currGameState = PLAYER_ONE_TURN;
                        window.close();
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

            if (currGameState == END_GAME_SCREEN) {
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

