#include <iostream>
#include <SFML/Graphics.hpp>
#include "board.h"
#include "rules.h"
#include "utility.h"




void drawBoard(sf::RenderWindow &window, short selectedPiece, short* gameState, short* movedPieceSquares)
{
    short column;
    short row;
    sf::Color lightSquare(131, 74, 9);
    sf::Color darkSquare(220, 188, 100);
    for (short i = 0; i < 64; i++){
        column = i % 8;
        row = i / 8; 
        sf::Color color = (column % 2 != row % 2) ? lightSquare : darkSquare;
        if (selectedPiece == i){
            color = sf::Color::Yellow;
        }
        else if(i == movedPieceSquares[0] || i == movedPieceSquares[1]){
            color = (i == movedPieceSquares[0]) ? sf::Color(50, 50, 250) : sf::Color(100, 100, 250);
        }
        // check if any white pieces attacks this sqaure. if so, color it yellow
        // for (short j = 0; j < 64; j++){
        //     if (getPieceColor(gameState[j]) == 1 && gameState[j] != 0){
        //         std::vector<short> possibleMoves = findSudoLegalMoves(gameState[j], j, gameState, 1);
        //         for (short k = 0; k < possibleMoves.size(); k++){
        //             if (possibleMoves[k] == i){
        //                 // color light and dark red
        //                 color = (color == lightSquare) ?  sf::Color(255, 50, 50) : sf::Color(255, 100, 100) ;
        //             }
        //         }
        //     }
        // }


            
        std::vector<short> possibleMoves = findAllLegalMoves(gameState[selectedPiece], selectedPiece, gameState, 1);
        for (short k = 0; k < possibleMoves.size(); k++){
            if (possibleMoves[k] == i){
                // color light and dark red
                color = (color == lightSquare) ?  sf::Color(255, 50, 50) : sf::Color(255, 100, 100) ;
            }
        }

        


        
        sf::RectangleShape square(sf::Vector2f(100, 100));
        square.setFillColor(color);
        square.setPosition(column * 100, row * 100);
        window.draw(square);
    }
}


void drawPiece(sf::RenderWindow &window, short piece, short location){

    sf::Texture texture;
    switch (piece)
    {
        case 1:
            texture.loadFromFile("./images/Chess_pdt60.png");
            break;
        case 2:
            texture.loadFromFile("./images/Chess_ndt60.png");
            break;
        case 3:
            texture.loadFromFile("./images/Chess_bdt60.png");
            break;
        case 4:
            texture.loadFromFile("./images/Chess_rdt60.png");
            break;
        case 5:
            texture.loadFromFile("./images/Chess_qdt60.png");
            break;
        // enpassant pawn
        case 7:
            texture.loadFromFile("./images/Chess_pdt60.png");
            break;
        case 8:
            texture.loadFromFile("./images/Chess_rdt60.png");
            break;
        case 6:
            texture.loadFromFile("./images/Chess_kdt60.png");
            break;
        case 9:
            texture.loadFromFile("./images/Chess_plt60.png");
            break;
        case 10:
            texture.loadFromFile("./images/Chess_nlt60.png");
            break;
        case 11:
            texture.loadFromFile("./images/Chess_blt60.png");
            break;
        case 12:
            texture.loadFromFile("./images/Chess_rlt60.png");
            break;
        case 13:    
            texture.loadFromFile("./images/Chess_qlt60.png");
            break;
        case 14:
            texture.loadFromFile("./images/Chess_klt60.png");
            break;
        // enpassant pawn
        case 15:
            texture.loadFromFile("./images/Chess_plt60.png");
            break;
        case 16:
            texture.loadFromFile("./images/Chess_rlt60.png");
            break;
    }
    texture.setSmooth(true);
    sf::Sprite sprite;  
    sprite.setTexture(texture);
    sprite.scale(sf::Vector2f(1.55f, 1.55f));   
    short* locationArr = convertIndexTo2D(location);
    sprite.setPosition((locationArr[1]) * 100, locationArr[0] * 100);
    window.draw(sprite);

}


void drawGameState(sf::RenderWindow &window, short* gameState)
{
    for (short i = 0; i < 64; i++) {
        drawPiece(window, gameState[i], i);
    }

}


short getSquareSelection(sf::RenderWindow& window) {
    short* location = new short[2];
    short returnValue = -1;

    sf::Vector2i mouseLocation = sf::Mouse::getPosition(window);
    location[0] = mouseLocation.x / 100;
    location[1] = mouseLocation.y / 100;

    returnValue = convertIndexTo1D(location[1], location[0]);

    delete[] location;

    return returnValue;
}


short getSelectedPiece(sf::RenderWindow& window, short* gameState){

    short location = getSquareSelection(window);

    if(gameState[location] != 0){
        return location;
    }
    return -1;
}


void handleUserInput(sf::RenderWindow& window, short* &gameState, short& turn, short& selectedPieceSquare, short colorAtBottom, short* &previousGameState, short &playersColor, short* movedPieceSquares){

    // ask if pawn of color as color at bottom reacheqs end of board. skip over other input code if so and open pawn promotion window. 

    short promoteLocation = checkForPawnPromotion(gameState, colorAtBottom);
    if(promoteLocation != -1){
        short promoteColor = getPieceColor(gameState[promoteLocation]);
        if (promoteColor == colorAtBottom){
            pawnPromotion(promoteColor, gameState, promoteLocation, turn);
            return;
        }
    }


    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {

        short selectedSquare = getSquareSelection(window);

        // this is piece selection
        if(getPieceColor(gameState[selectedSquare]) == turn && gameState[selectedSquare] != 0) 
        {
            // copy game state into previous game state

            previousGameState = new short[64];
            for (short i = 0; i < 64; i++){
                previousGameState[i] = gameState[i];
            } 
            // check if attemoting castle
            if ((gameState[selectedPieceSquare] == 6 && gameState[selectedSquare] == 8 || gameState[selectedPieceSquare] == 14 && gameState[selectedSquare] == 16) && isCheck(gameState, turn) == false){
                attemptMove(gameState, selectedPieceSquare, selectedSquare, turn, colorAtBottom, movedPieceSquares);
                return;
            } 
            // if not castle switch selected
            else {
                selectedPieceSquare = selectedSquare;

            }
        }
        // this is attempted move
        else if(selectedPieceSquare != -1)
        {
            attemptMove(gameState, selectedPieceSquare, selectedSquare, turn, colorAtBottom, movedPieceSquares);  
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
        gameState = buildStartPosition(turn);    
        turn = 1;
        selectedPieceSquare = -1;
        movedPieceSquares[0] = -1;
        movedPieceSquares[1] = -1;
        std::cout << "reset" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)){
        gameState = previousGameState;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
        playersColor = !playersColor;
    }
}


short checkForPawnPromotion(short* &gameState, short colorAtBottom){
    for(int i = 0; i < 8; i++){
        if(gameState[i] == 1 && colorAtBottom == 0 || gameState[i] == 9 && colorAtBottom == 1){
            return i;
        }
        if (gameState[i + 56] == 9 && colorAtBottom == 0 || gameState[i + 56] == 1 && colorAtBottom == 1){
            return i + 56;
        }
    }
    return -1;
}


void pawnPromotion(short color, short* &gameState, short location, short &turn){

    sf::RenderWindow window(sf::VideoMode(200, 200), "select promotion");

    while (window.isOpen())
    {
        window.clear();
        sf::RectangleShape square(sf::Vector2f(200, 200));
        if (color == 0){
            square.setFillColor(sf::Color::White);
            window.draw(square);
            drawPiece(window, 2, 0);
            drawPiece(window, 3, 1);
            drawPiece(window, 4, 8);
            drawPiece(window, 5, 9);
        }
        else {
            square.setFillColor(sf::Color::Black);
            window.draw(square);
            drawPiece(window, 10, 0);
            drawPiece(window, 11, 1);
            drawPiece(window, 12, 8);
            drawPiece(window, 13, 9);
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            short selectedSquare = getSquareSelection(window);
            if (selectedSquare == 0 || selectedSquare == 1 || selectedSquare == 8 || selectedSquare == 9){
                short choice;
                choice = (selectedSquare < 3) ? selectedSquare + 2 : selectedSquare - 4;
                gameState[location] = choice + color * 8;
                turn = !turn;   
                window.close();
            }
        }


        window.display();
    }
}
