#pragma once


std::string getChessCoordinate(short position);

void attemptMoveTest(short* &gameState, short &pieceSquare, short targetSquare, short &turn, short colorAtBottom);



int testMoveCount2(short* gameState, int depth, short turn, short move, int totalDepth);