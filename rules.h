

std::vector<short> findSudoLegalMoves(short item, short location, short* gameState, short colorAtBottom);

std::vector<short> findAllLegalMoves(short item, short location, short* gameState, short colorAtBottom);


void findLegalSlidingMoves(short item, short location, short* gameState, std::vector<short> &legalMoves);


void findLegalNightMoves(short item, short location, short* gameState, std::vector<short> &legalMoves);


void findLegalPawnMoves(short item, short location, short* gameState, std::vector<short> &legalMoves, short colorAtBottom);




void findLegalKingMoves(short item, short location, short* gameState, std::vector<short> &legalMoves);



void removeIllegalMoves(short* gameState, short location, short color, std::vector<short> &legalMoves);


bool isCheck(short* gameState, short color);
