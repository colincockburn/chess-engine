#include <iostream>
#include <vector> 
#include "rules.h"
#include "board.h"
#include "utility.h" 




std::vector<short> findSudoLegalMoves(short item, short location, short* gameState, short colorAtBottom){

    
    std::vector<short> legalMoves;
    if(item >= 3 && item <= 5 || item >= 11 && item <= 13 || item == 16 || item == 8){
        findLegalSlidingMoves(item, location, gameState, legalMoves);
    }

    if (item == 2 || item == 10){
        findLegalNightMoves(item, location, gameState, legalMoves);
    }
    
    if (item == 1 || item == 9 || item == 7 || item == 15){
        findLegalPawnMoves(item, location, gameState, legalMoves, colorAtBottom);
    }
    if (item == 6 || item == 14){
        findLegalKingMoves(item, location, gameState, legalMoves);
    }



    return legalMoves;
}


std::vector<short> findAllLegalMoves(short item, short location, short* gameState, short colorAtBottom){

    std::vector<short> legalMoves = findSudoLegalMoves(item, location, gameState, colorAtBottom);

    removeIllegalMoves(gameState, location, getPieceColor(item), legalMoves);

    return legalMoves;}


void findLegalSlidingMoves(short item, short location, short* gameState, std::vector<short>& legalMoves) {

    short color = getPieceColor(item);
    short step;

    for (short j = 0; j < 8; j++) {
        // 0 = right, 1 = left, 2 = up, 3 = down, 4 = up right, 5 = up left, 6 = down right, 7 = down left
        switch (j) {
            case 0:
                step = 1;
                break;
            case 1:
                step = -1;
                break;
            case 2:
                step = 8;
                break;
            case 3:
                step = -8;
                break;
            case 4:
                step = 9;
                break;
            case 5:
                step = -9;
                break;
            case 6:
                step = 7;
                break;
            case 7:
                step = -7;
                break;
        }

        // Skip to the next loop or break the loop if the item is incapable of making that move.
        if ((j <= 3 && (item == 3 || item == 11)) || (j >= 4 && (item == 4 || item == 12 || item == 8 || item == 16))) {
            continue;
        }

        // Check if the move is legal
        short target;
        for (short i = 1; i <= 7; i++) {
            target = location + step * i;

            short targetRow = target / 8;  // Get the target row
            short targetCol = target % 8;  // Get the target column

            // Check if the target position is out of bounds
            if (target < 0 || target > 63) {
                break;
            }

            if ((std::abs(target / 8 - location / 8) > 1 || std::abs(target % 8 - location % 8) > 1) && i == 1) {
                break;
            }

            if (gameState[target] == 0) {
                legalMoves.push_back(target);
            } else if (getPieceColor(gameState[target]) != color) {
                legalMoves.push_back(target);
                break;
            } else {
                break;
            }


            short targetNextStep = target + step;
            short targetNextRow = targetNextStep / 8;  // Get the target row
            short targetNextCol = targetNextStep % 8;


            if (std::abs(targetRow - targetNextStep / 8) > 1 || std::abs(targetCol - targetNextStep % 8) > 1) {
                break;
            }
        }
    }
}



void findLegalNightMoves(short item, short location, short* gameState, std::vector<short>& legalMoves) {
    short legalSteps[8] = {6, 15, 17, 10, -6, -15, -17, -10};

    short row = location / 8;  // Get the current row
    short col = location % 8;  // Get the current column

    for (short i = 0; i < 8; i++) {
        short target = location + legalSteps[i];

        short targetRow = target / 8;  // Get the target row
        short targetCol = target % 8;  // Get the target column

        // Check if the target position is out of bounds or crosses the board edge
        if (target < 0 || target > 63 || std::abs(targetRow - row) > 2 || std::abs(targetCol - col) > 2) {
            continue;
        }

        if (gameState[target] == 0 || getPieceColor(gameState[target]) != getPieceColor(item)) {
            legalMoves.push_back(target);
        }
    }
}


void findLegalPawnMoves(short item, short location, short* gameState, std::vector<short> &legalMoves, short colorAtBottom){


    short color = getPieceColor(item);
    // top pawn
    if (colorAtBottom != color){
        // double push
        if (location >= 8 && location <= 15){
            if (gameState[location + 16] == 0 && gameState[location + 8] == 0){
                legalMoves.push_back(location + 16);
            }
        }
        if (gameState[location + 8] == 0){
            legalMoves.push_back(location + 8);
        }
        //handle attack
        if (location % 8 != 0 && gameState[location + 7] != 0 && getPieceColor(gameState[location + 7]) != color){
            legalMoves.push_back(location + 7);
        }
        if (location % 8 != 7 && gameState[location + 9] != 0 && getPieceColor(gameState[location + 9]) != color){
            legalMoves.push_back(location + 9);
        }
        // handle  enpassant
        // doesn't work if you're able attempt an enpassant on your own piece
        if (location % 8 != 0 && (gameState[location - 1] == 7 || gameState[location - 1] == 15)){
            legalMoves.push_back(location + 7);
        }
        if (location % 8 != 7 && (gameState[location + 1] == 7 || gameState[location + 1] == 15)){
            legalMoves.push_back(location + 9);
        } 
        

    }
    // bottom pawn
    else{
        // double push
        if (location >= 48 && location <= 55){
            if (gameState[location - 16] == 0 && gameState[location - 8] == 0){
                legalMoves.push_back(location - 16);
            }
        }
        if (gameState[location - 8] == 0){
            legalMoves.push_back(location - 8);
        }
        //handle attack
        if (location % 8 != 0 && gameState[location - 9] != 0 && getPieceColor(gameState[location - 9]) != color){
            legalMoves.push_back(location - 9);
        }
        if (location % 8 != 7 && gameState[location - 7] != 0 && getPieceColor(gameState[location - 7]) != color){
            legalMoves.push_back(location - 7);
        }

        // handle  enpassant
        // doesn't work if you're able attempt an enpassant on your own piece, currently you can't attack your own piece
        if (location % 8 != 0 && (gameState[location - 1] == 15 || gameState[location - 1] == 7)){
            legalMoves.push_back(location - 9);
            // std::cout << "enpassant 3" << std::endl;
        }
        if (location % 8 != 7 && (gameState[location + 1] == 15 || gameState[location + 1] == 7)){
            legalMoves.push_back(location - 7);
        }
        
        
    }
        // might want this later
        // if (location % 8 != 0 && gameState[location + 7] != 0 && getPieceColor(gameState[location + 7]) != color){
        //     legalMoves.push_back(location + 7);
        // }
        // if (location % 8 != 7 && gameState[location + 9] != 0 && getPieceColor(gameState[location + 9]) != color){
        //     legalMoves.push_back(location + 9);
        // }

}



void findLegalKingMoves(short item, short location, short* gameState, std::vector<short>& legalMoves) {

    std::vector<short> legalSteps = {1, 9, 8, 7, -1, -9, -8, -7};

    short row = location / 8;  // Get the current row
    short col = location % 8;  // Get the current column

    for (short i = 0; i < 8; i++) {
        short target = location + legalSteps[i];

        short targetRow = target / 8;  // Get the target row
        short targetCol = target % 8;  // Get the target column

        // Check if the target position is out of bounds or crosses the board edge
        if (target < 0 || target > 63 || std::abs(targetRow - row) > 1 || std::abs(targetCol - col) > 1) {
            continue;
        }

        if (gameState[target] == 0 || getPieceColor(gameState[target]) != getPieceColor(item)) {
            legalMoves.push_back(target);
        }
    }
    // Rest of the code...


        // handle castle
        // short castle
        // this is expensive. find asnwer from somewhere else and store it
        if (gameState[location + 3] == 8 || gameState[location + 3] == 16){
            if (gameState[location + 1] == 0 && gameState[location + 2] == 0){
                legalMoves.push_back(location + 3);
            }
        }
        // long castle
        if (gameState[location - 4] == 8 || gameState[location - 4] == 16){
            if (gameState[location - 1] == 0 && gameState[location - 2] == 0 && gameState[location - 3] == 0){
                legalMoves.push_back(location - 4);
            }
        }
}



// this could be improved by only checking the squares that are being attacked by the other team
bool isCheck(short* gameState, short color){


    for (int  i = 0; i < 64; i++){
        if (gameState[i] != 0 && getPieceColor(gameState[i]) != color){
            
            //only works for white ar bottom
            std::vector<short> legalMoves = findSudoLegalMoves(gameState[i], i, gameState, 1);
            
           
            for (int j = 0; j < legalMoves.size(); j++){
            
                if (gameState[legalMoves[j]] == 6 && color == 0 || gameState[legalMoves[j]] == 14 && color == 1){
                    
                    return true;
                }
            }
        }
    }
    return false;
}


void removeIllegalMoves(short* gameState, short location, short color, std::vector<short> &legalMoves){

    // remove moves that put that leave king in chekc
    for (int i = 0; i < legalMoves.size(); i++){

        if((gameState[legalMoves[i]] == 8 && gameState[location] == 6 || gameState[legalMoves[i]] == 16 && gameState[location] == 14 ) && isCheck(gameState, color)){
            legalMoves.erase(legalMoves.begin() + i);
            i--;
            continue;
        }

        short* tempGameState = new short[64];
        for (int j = 0; j < 64; j++){
            tempGameState[j] = gameState[j];
        }
        // handle castle case
        if (gameState[legalMoves[i]] == 8 && gameState[location] == 6 || gameState[legalMoves[i]] == 16 && gameState[location] == 14){
            handleCastle(tempGameState, location, legalMoves[i]);
        }
        else{
            tempGameState[legalMoves[i]] = tempGameState[location];
            tempGameState[location] = 0;

        }
        bool checked = isCheck(tempGameState, color);
        if (checked){
            legalMoves.erase(legalMoves.begin() + i);
            i--;
        }

        delete[] tempGameState;
    }

    
}







