#pragma once
#include <SFML/Graphics.hpp>


void drawBoard(sf::RenderWindow &window, short selectedPiece,   short* gameState, short* movedPieceSquares);


void drawPiece(sf::RenderWindow &window, short piece, short location);


void drawGameState(sf::RenderWindow &window, short* gameState);


short getSquareSelection(sf::RenderWindow& window);


short getSelectedPiece(sf::RenderWindow& window, short* gameState);


void handleUserInput(sf::RenderWindow& window, short* &gameState, short& turn, short& selectedPieceSquare, short colorAtBottom, short* &previousGameState, short &playersColor, short* movedPieceSquares);


short checkForPawnPromotion(short* &gameState, short colorAtBottom);


void pawnPromotion(short color, short* &gameState, short location, short& turn);
