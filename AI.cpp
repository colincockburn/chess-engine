#include <iostream>
#include <random>
#include <vector>
#include "board.h"
#include "rules.h"
#include "AI.h"
#include <chrono>
#include "utility.h"


PositionTables positionTables;


void checkAndQueenPromote(short* &gameState, short colorAtBottom){
    short promoteLocation = checkForPawnPromotion(gameState, colorAtBottom);   
    if (promoteLocation != -1){
        short color = getPieceColor(gameState[promoteLocation]);
        gameState[promoteLocation] = color == 0 ? 5 : 13;
    }
}


short getItemValue(short item)
{
    switch (item)
    {
    case 1:
        return 100;
    case 2:
        return 300;
    case 3:
        return 300;
    case 4:
        return 500;
    case 5:
        return 900;
    case 6:
        return 1;
    case 7:
        return 100;   
    case 8:
        return 500;
    case 9:
        return 100;
    case 10:
        return 300;
    case 11:
        return 300;
    case 12:
        return 500; 
    case 13:
        return 900;
    case 14:
        return 1;
    case 15:
        return 100;
    case 16:
        return 500;
    }
    return 0;
}


void evaluatePosition(short item, short location, short& score){
    

    switch (item)
    {   
    case 1:
        score += positionTables.topPawnTable[location];
        break;
    case 2:
        score += positionTables.knightTable[location];
        break;
    case 3:
        score += positionTables.topBishopTable[location];
        break;
    case 4:
        score += positionTables.topRookTable[location];
        break;
    case 5:
        score += positionTables.queenTable[location];
        break;
    case 6:
        score += positionTables.topKingTable[location];
        break;
    case 7:
        score += positionTables.topPawnTable[location];
        break;
    case 8:
        score += positionTables.topRookTable[location];
        break;
    case 9:
        score += positionTables.bottomPawnTable[location];
        break;
    case 10:
        score += positionTables.knightTable[location];
        break;
    case 11:
        score += positionTables.bottomBishopTable[location];
        break;
    case 12:
        score += positionTables.bottomRookTable[location];
        break;
    case 13:
        score += positionTables.queenTable[location];
        break;
    case 14:    
        score += positionTables.bottomKingTable[location];
        break;
    case 15:
        score += positionTables.bottomPawnTable[location];
        break;
    case 16:    
        score += positionTables.bottomRookTable[location];
        break;
    }
}


short countMaterial(short* gameState, short color){
    short endGameMaterialCount = 0;
    for (int i = 0; i < 64; i++){
        if (getPieceColor(gameState[i]) == color){
            short endGameScore = getItemValue(gameState[i]);
            endGameMaterialCount += endGameScore;
        }
    }
    short material = 0;
    for (int i = 0; i < 64; i++){
        if (getPieceColor(gameState[i]) == color){
            short score = getItemValue(gameState[i]);
            if((gameState[i] == 6 || gameState[i] == 14 ) && endGameMaterialCount < 12){
                score += positionTables.kingEndGameTable[i];
            }
            else{
                evaluatePosition(gameState[i], i, score);
            }
            material += score;
        }
    }
    return material;
}


short evaluate(short* gameState, short turn){
    short material = countMaterial(gameState, turn) - countMaterial(gameState, !turn);
    return material;
}


short evaluationSearch(short* gameState, short turn, short depth, short totalDepth, short alpha, short beta, Move &bestMove, bool& gameOver)
{
    if (depth <= -1){
        return evaluate(gameState, turn);
    }

    bool foundAttackingMove = false;
    bool moveFound = false;
    for (short i = 0; i < 64; i++){
        if (gameState[i] == 0 || getPieceColor(gameState[i]) != turn){
            continue;
        }

        std::vector<short> possibleMoves = findAllLegalMoves(gameState[i], i, gameState, 1);
        if (possibleMoves.size() != 0){
            moveFound = true;
        }
        // loop through each move
        for(short target : possibleMoves){

            if (gameState[target] == 0 && depth <= 0){
                continue; 
            }
            else if(gameState[target] != 0 && depth <= 0){
                foundAttackingMove = true;
            }

            short* newGameState = new short[64];
            for (int k = 0; k < 64; k++) {
                newGameState[k] = gameState[k];
            }

            move(newGameState, i, target, turn, 1);
            checkAndQueenPromote(newGameState, 1 );
            short evaluation = -evaluationSearch(newGameState, !turn, depth - 1, totalDepth, -beta, -alpha, bestMove, gameOver);
            if (i == 21 && target == 53 && depth == totalDepth){
                std::cout << "current move: " << i << " " << target << " " << evaluation << std::endl;
            }
            delete[] newGameState;


            if(evaluation >= beta){
                return beta;
            }

            if (evaluation > alpha){
                alpha = evaluation;
                if (depth == totalDepth){
                    bestMove.start = i;
                    bestMove.end = target;
                    bestMove.score = evaluation;     
                }
            }
        }
    }
    if (!moveFound){
        if (depth == totalDepth){
            gameOver = true;
            return 0;
        }
        if (isCheck(gameState, turn)){
            if(depth == totalDepth - 1){
                return -8001;
            }
            return -8000;
        }
        else {
            return 0;
        }
    }

    if (!foundAttackingMove && depth <= 0){
        return evaluate(gameState, turn);
    } 
    return alpha;
}


void getBestMoveWithTimeLimit(short* gameState, short turn, short colorAtBottom, short timeLimit, Move &bestMove, bool& gameOver){
    auto startTime = std::chrono::steady_clock::now();  // Record the start time

    short beta = 10001;
    short alpha = -10000;
    short depth = 1;
    
    while (true){
        
        depth++;
        evaluationSearch(gameState, turn, depth, depth, alpha, beta, bestMove, gameOver);
        auto endTime = std::chrono::steady_clock::now();  // Record the end time
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        if (duration.count() > timeLimit || gameOver){
            std::cout << "depth: " << depth << std::endl;
            break;
        }
    }
}


void makeCalculatedMove(short* &gameState, short& turn, short colorAtBottom, short* movedPieceSquares){
    std::vector<short> possibleMoves;
    Move bestMove;
    bestMove.score = -10000;
    bool gameOver = false;
    getBestMoveWithTimeLimit(gameState, turn, colorAtBottom, 400, bestMove, gameOver);

    if (gameOver){
        std::cout << "game over" << std::endl;
        return;
    }


    movedPieceSquares[0] = bestMove.start;
    movedPieceSquares[1] = bestMove.end;
    move(gameState, bestMove.start, bestMove.end, turn, colorAtBottom);
    turn = !turn; 
    checkAndQueenPromote(gameState, colorAtBottom);
}


void computerInput(short* &gameState, short& turn, short colorAtBottom, short* movedPieceSquares) {

    auto startTime = std::chrono::steady_clock::now();  // Record the start time
    makeCalculatedMove(gameState, turn, colorAtBottom, movedPieceSquares);

    auto endTime = std::chrono::steady_clock::now();  // Record the end time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Move took " << duration.count() << " milliseconds." << std::endl;
}


