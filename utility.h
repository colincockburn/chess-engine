void printPiece(short piece);


short getPieceColor(short piece);


void printGameState(short* gameState);


short convertIndexTo1D(short row, short column);


short* convertIndexTo2D(short index);

short* buildStartPosition(short &turn);


short* buildFenCode(std::string &fen, short &turn);


short* createGameStateArray();


void attemptMove(short* &gameState, short &pieceSquare, short targetSquare, short &turn, short colorAtBottom, short* movedPieceSquares);


void move(short* &gameState, short &pieceSquare, short targetSquare, short &turn, short colorAtBottom);


void handleEnpessant(short* &gameState, short pieceSquare, short targetSquare, short colorAtBottom);


void handleCastle(short* &gameState, short pieceSquare, short targetSquare);



void removeCastleRooks(short* &gameState, short color, short piece, short location);






