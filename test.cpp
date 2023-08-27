#include <iostream>
#include <random>
#include <vector>
#include "board.h"
#include "rules.h"
#include "AI.h"
#include <chrono>
#include "test.h"
#include "utility.h"




std::string getChessCoordinate(short position) {
    int row = 7 - (position / 8);
    int col = position % 8;
    char letter = 'a' + col;
    int number = row + 1;

    std::string coordinate;
    coordinate += letter;
    coordinate += std::to_string(number);
    return coordinate;
}


void testMoveCount2Helper(int depth){
    std::string fenCode = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";
    short turn;
    short* gameState = buildFenCode(fenCode, turn);
    std::cout << "turn:" << turn << std::endl;
    printGameState(gameState);

    std::cout << testMoveCount2(gameState, depth, turn, -1, depth) << std::endl;
    
}


int testMoveCount2(short* gameState, int depth, short turn, short move, int totalDepth){
    if (depth == 0){
        return 0;
    }

    int counter = 0;


    std::vector<short> pieces ;

    for (short n =0; n < 64; n++){
        if (getPieceColor(gameState[n]) == turn){
            pieces.push_back(n);
        }   
    }

    

    for (int i = 0; i < pieces.size() ; i++) {
        std::vector<short> moves = findSudoLegalMoves(gameState[pieces[i]], pieces[i], gameState, 1);
        removeIllegalMoves(gameState, pieces[i], turn, moves);

        for (int j = 0; j < moves.size(); j++) {



            short* newGameState = new short[64];
            for (int k = 0; k < 64; k++) {
                newGameState[k] = gameState[k];
            }

            


  
            attemptMoveTest(newGameState, pieces[i], moves[j], turn, 1);


            if(depth == 1){
                counter += 1;
            }
            if(depth == totalDepth){
                std::cout << getChessCoordinate( pieces[i]) ;
                std::cout << getChessCoordinate(moves[j]);
                if (totalDepth == 1){
                    std::cout << ": " << counter << std::endl;


                }

            }
            counter += testMoveCount2(newGameState, depth - 1, !turn, moves[j], totalDepth);

            // //deallocate memory
            delete[] newGameState;
        }

        

    }
    if(depth == totalDepth - 1) {

        std::cout << ": " << counter << std::endl;
    }
    return counter;
}



void attemptMoveTest(short* &gameState, short &pieceSquare, short targetSquare, short &turn, short colorAtBottom)
{

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
    
}





int main(int argc, char** argv) {

    // run testcounter2helper using the command line argument as the depth
    testMoveCount2Helper(std::stoi(argv[1]));



    return 0;

}
