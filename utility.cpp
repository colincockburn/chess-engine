#include <iostream>
#include <vector> 
#include "rules.h"
#include "board.h"
#include "utility.h"


void printPiece(short piece)
{
    switch (piece)
    {
        case 0:
            std::cout << "Empty";
            break;
        case 1:
            std::cout << "black Pawn";
            break;
        case 2:
            std::cout << "black Knight";
            break;
        case 3:
            std::cout << "black Bishop";
            break;
        case 4:
            std::cout << "black Rook";
            break;
        case 5:
            std::cout << "black Queen";
            break;
        case 6:
            std::cout << "black King";
            break;
        case 7:
            std::cout << "black enpassant Pawn";
            break;
        case 8:
            std::cout << " black castle rook";
            break;
        case 9:
            std::cout << "white Pawn";
            break;
        case 10:
            std::cout << "white Knight";
            break;
        case 11:
            std::cout << "white Bishop";
            break;
        case 12:
            std::cout << "white Rook";
            break;
        case 13:    
            std::cout << "white Queen";
            break;
        case 14:
            std::cout << "white King";
            break;
        case 15:
            std::cout << "white enpassant Pawn";
            break;
        case 16:
            std::cout << "white castle rook";
            break;
    }
}


short getPieceColor(short piece){
    if (piece <= 8){
        return 0;
    }
    else 
    {
        return 1;
    }
}


void printGameState(short* gameState)
{
    for (short i = 0; i < 64; i++)
    {
        if (i % 8 == 0)
        {
            std::cout << std::endl;
        }
        std::cout << gameState[i] << " ";
    }
    std::cout << std::endl;
}


short convertIndexTo1D(short row, short column)
{
    return row * 8 + column;

}


short* convertIndexTo2D(short index)
{
    short* arr = new short[2];
    arr[0] = index / 8;
    arr[1] = index % 8;
    return arr;
    
}


short* buildStartPosition(short &turn){

    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq ";
    return buildFenCode(fen, turn);
}


//takes in string as parameter
short* buildFenCode(std::string &fen, short &turn) {

    short* gameState = new short[64];  // Allocate memory for the rows
    int i = 0;
    bool boardBuilt = false;
    bool turnsSet = false; 
    short column = -1;
    for (char c : fen) {
        if (c == ' ') {
            boardBuilt = true;
            continue;
        }
        else if (c == '/') {
            continue;
        }
        else if (c >= '0' && c <= '9') {
            for (int j = 0; j < c - '0'; j++) {
                gameState[i] = 0;
                if(i< 64) i++;
            }
        }
        else {
            switch (c) {
                case 'p':
                    gameState[i] = 1;
                    break;
                case 'n':
                    gameState[i] = 2;
                    break;
                case 'b':
                    gameState[i] = 3;
                    break;
                case 'r':
                    gameState[i] = 4;
                    break;
                case 'q':
                    gameState[i] = 5;
                    break;
                case 'k':
                    gameState[i] = 6;
                    break;
                case 'P':
                    gameState[i] = 9;
                    break;
                case 'N':
                    gameState[i] = 10;
                    break;
                case 'B':
                    gameState[i] = 11;
                    break;
                case 'R':
                    gameState[i] = 12;
                    break;
                case 'Q':
                    gameState[i] = 13;
                    break;
                case 'K':
                    gameState[i] = 14;
                    break;
            }
            if(i<64)i++;
        }
        if (turnsSet){
            if (c == 'Q'){
                gameState[56]= 16;
            }
            else if (c == 'K'){
                gameState[63] = 16;
            }
            else if (c == 'q'){
                gameState[0] = 8;
            }
            else if (c == 'k'){
                gameState[7] = 8;
            }
            if (column !=-1){

                short row = c- '0';
                row = (row-8)*-1;
                short location = convertIndexTo1D(row, column);
                if (location / 8 == 2){
                    gameState[location + 8] = 7;
                }
                else if (location / 8 == 5){
                    gameState[location - 8] = 15;
                }
                return gameState;
            }
            switch(c){                
                
                default:
                    column = -1;
                    break;

                case('a'):
                    column = 0;
                    break;
                case('b'):
                    column = 1;
                    break;
                case('c'):
                    column = 2;
                    break;
                case('d'):
                    column = 3;
                    break;
                case('e'):
                    column = 4;
                    break;
                case('f'):
                    column = 5;
                    break;
                case('g'):
                    column = 6;
                    break;
                case('h'):
                    column = 7;
                    break;
            }

        }
        if (boardBuilt && !turnsSet){
            if (c == 'w'){
                turn = 1;
                turnsSet = true;
            }
            else if (c == 'b'){
                turn = 0;
                turnsSet = true;
            }
        }
        
    }
    return gameState;   
}


short* createGameStateArray() {
    short* arr = new short[64];  // Allocate memory for the rows

    for (short i = 0; i < 64; i++) {
       arr[i] = 0;
    }

    // Assign chess piece value Pawns
    for (short i = 0; i < 8; i++) {
        arr[i+8] = 1;
        arr[i + 48] = 9;
    }

    // Knights
    arr[1] = 2;
    arr[6] = 2;
    arr[57] = 10;
    arr[62] = 10;

    // Bishops
    arr[2] = 3;
    arr[5] = 3;
    arr[58] = 11;
    arr[61] = 11;

    // Rooks
    arr[0] = 8;
    arr[7] = 8;
    arr[56] = 16;
    arr[63] = 16;

    // Queens
    arr[3] = 5;
    arr[59] = 13;
    
    // Kings
    arr[4] = 6;
    arr[60] = 14;

    return arr;  // Return the pointer to the two-dimensional array
}


void attemptMove(short* &gameState, short &pieceSquare, short targetSquare, short &turn, short colorAtBottom, short* movedPieceSquares)
{


    std::vector<short> legalMoves = findSudoLegalMoves(gameState[pieceSquare], pieceSquare, gameState, colorAtBottom);

    removeIllegalMoves(gameState, pieceSquare, turn, legalMoves);


    if(std::find(legalMoves.begin(), legalMoves.end(), targetSquare) != legalMoves.end()){

        // handle enpassant
        handleEnpessant(gameState, pieceSquare, targetSquare, colorAtBottom);

        // handle castle
        if (gameState[targetSquare] == 8 && gameState[pieceSquare] == 6 || gameState[targetSquare] == 16 && gameState[pieceSquare] == 14){
            handleCastle(gameState, pieceSquare, targetSquare);
        }
        else{
            gameState[targetSquare] = gameState[pieceSquare];
            gameState[pieceSquare] = 0;
        }

        removeCastleRooks(gameState, turn, gameState[targetSquare], targetSquare);

        // move the item to the target square
        
        turn = (turn + 1) % 2;
        movedPieceSquares[0] = pieceSquare;
        movedPieceSquares[1] = targetSquare;
        pieceSquare = -1;

    }
}


void move(short* &gameState, short &pieceSquare, short targetSquare, short &turn, short colorAtBottom){
        // handle enpassant
        handleEnpessant(gameState, pieceSquare, targetSquare, colorAtBottom);

        // handle castle
        if (gameState[targetSquare] == 8 && gameState[pieceSquare] == 6 || gameState[targetSquare] == 16 && gameState[pieceSquare] == 14){
            handleCastle(gameState, pieceSquare, targetSquare);
        }
        else{
            gameState[targetSquare] = gameState[pieceSquare];
            gameState[pieceSquare] = 0;
        }

        removeCastleRooks(gameState, turn, gameState[targetSquare], targetSquare);
}


void handleEnpessant(short* &gameState, short pieceSquare, short targetSquare, short colorAtBottom){
    // before setting a new ennpessant, remove any on the board
    for (short i = 0; i < 64; i++){
        if (gameState[i] == 7){
            gameState[i] = 1;
        }
        if (gameState[i] == 15){
            gameState[i] = 9;
        }
    }

    // if the move is a pawn moving two forward, changed the pawn to an oppose pawn
    // this changes the pan to a enpassant pawn before it is moved.
    if (gameState[pieceSquare] == 1 && targetSquare - pieceSquare == 16)     
    {
        gameState[pieceSquare] = 7;
    }
    else if (gameState[pieceSquare] == 9 && pieceSquare - targetSquare == 16){
        gameState[pieceSquare] = 15;
    }

    // if the move is a pawn is making a diagonal move on to an empty square there must be an enpassant
    // remove pawn that is being attacked
    if ((gameState[pieceSquare] == 1 || gameState[pieceSquare] == 9)){

        if (gameState[targetSquare] == 0 && targetSquare % 8 != pieceSquare % 8){

            if (getPieceColor(gameState[pieceSquare]) != colorAtBottom){
                gameState[targetSquare - 8] = 0;
            }
            else{
                gameState[targetSquare + 8] = 0;
            }
        }

    }
}


void handleCastle(short* &gameState, short pieceSquare, short targetSquare){

        // short castle
        if (targetSquare - pieceSquare == 3){

            gameState[pieceSquare + 1] = gameState[pieceSquare + 3];
            gameState[pieceSquare + 3] = 0;
            gameState[pieceSquare + 2] = gameState[pieceSquare];
            gameState[pieceSquare] = 0;
        }
        // long castle
        else if (targetSquare - pieceSquare == - 4){
            gameState[pieceSquare - 1] = gameState[pieceSquare - 4];
            gameState[pieceSquare - 4] = 0;
            gameState[pieceSquare - 2] = gameState[pieceSquare];
            gameState[pieceSquare] = 0;
        }

}


void removeCastleRooks(short* &gameState, short color, short piece, short location){

    if(piece == 6 || piece == 14){
        for (int i = 0; i < 64; i++){
            if (gameState[i] == 8 && color == 0){
                gameState[i] = 4;
            }
            else if(gameState[i] == 16 && color == 1){
                gameState[i] = 12;
            }
        }
    }
    if(piece == 8){
        gameState[location] = 4;
    }
    else if(piece == 16){
        gameState[location] = 12;
    }
}

