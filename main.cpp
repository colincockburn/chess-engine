#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.h"
#include "rules.h"
#include "AI.h"
#include "utility.h"



int main()
{

    short selectedPieceSquare = -1;
    short* movedPieceSquares = new short[2];

    // white is 1, black is 0
    // std::string fenCode = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ";
    // std::string fenCode = "6pk/6p1/8/1P2r3/8/8/PPQ3p1s/KP w";
    // std::string fenCode = "rnb2k1r/ppq1bppp/2p5/8/2B5/2N5/PPP2KPP/RNBQ3R w";
    // std::string fenCode = "2kr4/ppn3p1/5nN1/8/2Q5/8/P1PP1PPP/b1BK4 B";
    short turn = 1;
    short playersColor = 1;
    // short* gameState = buildFenCode(fenCode, turn);
    short* gameState = buildStartPosition(turn);
    short* previousGameState;
    short colorAtBottom = 1;


    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Board Prototype 1.0");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // don't change turn away from player until they promote their pawn
        short promoteLocation = checkForPawnPromotion(gameState, colorAtBottom);
        if (promoteLocation != -1){
            if (promoteLocation / 8 == 0){
                turn = colorAtBottom;
            }
            else {
                turn = !colorAtBottom;
            }
        }
        
        if (turn == playersColor){
             handleUserInput(window, gameState, turn, selectedPieceSquare, colorAtBottom, previousGameState, playersColor, movedPieceSquares);
        }
        else {
            computerInput(gameState, turn, colorAtBottom, movedPieceSquares);
        }
        // handle logic for piece selection
       

        // display changes
        drawBoard(window, selectedPieceSquare, gameState, movedPieceSquares);
        drawGameState(window, gameState);
        window.display();
    }

    return 0;
}