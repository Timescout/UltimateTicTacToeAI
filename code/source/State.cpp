#include "State.h"
#include <stdexcept>
#include <math.h>

///// move struct definitions /////

void move::init(activeBoard moveBoard, uint8_t moveSpace)
{
    if (moveBoard > activeBoard::board8 or moveSpace > 8) 
    {
        throw std::out_of_range("Tried to create move outside of board");
    }
    board = moveBoard;
    space = moveSpace;
}

move::move()
{
    init(activeBoard::board0, 0);
}

move::move(activeBoard moveBoard, uint8_t moveSpace) 
{
    init(moveBoard, moveSpace);
}

move::move(uint8_t binary)
{
    init(activeBoard(binary >> 4), binary & 0b1111);
}

uint8_t move::toBinary() const
{
    return (board << 4) + space;
}

///// evaluationValue definitions /////

void evaluationValue::init(player toWin, int depthToWin)
{
    playerToWin = toWin;
    depth = depthToWin;
}

evaluationValue::evaluationValue()
{
    init(player::neither, 0);
}

evaluationValue::evaluationValue(player toWin, int depthToWin)
{
    init(toWin, depthToWin);
}

bool evaluationValue::operator==(const evaluationValue& other) const
{
    return playerToWin == other.playerToWin && depth == other.depth;
}

bool evaluationValue::operator!=(const evaluationValue& other) const
{
    return !(*this == other);
}

bool evaluationValue::operator>(const evaluationValue& other) const
{
    int value, othersValue;
    // check that Depth is within acceptable range, 0 to 81 inclusive.
    if (0 > depth or depth > 81 or 0 > other.depth or other.depth > 81)
    { 
        throw std::invalid_argument("Passed evaluationValue with invalid depth");
    }

    // determine the value of this state.
    switch (playerToWin)
    {
    case player::x:
        value = 100;
        value -= depth;
        break;
    case player::o:
        value = -100;
        value += depth;
        break;
    case player::draw:
        value = 0;
        break;
    case player::neither:
        throw std::invalid_argument("Passed indeterminant evaluationValue into comparison");
        break;
    }

    // determine the other's value
    switch (other.playerToWin)
    {
    case player::x:
        othersValue = 100;
        othersValue -= other.depth;
        break;
    case player::o:
        othersValue = -100;
        othersValue += other.depth;
        break;
    case player::draw:
        othersValue = 0;
        break;
    case player::neither:
        throw std::invalid_argument("Passed indeterminant evaluationValue into comparison");
        break;
    }

    return value > othersValue;
}

bool evaluationValue::operator>=(const evaluationValue& other) const
{
    return *this > other || *this == other;
}

bool evaluationValue::operator<(const evaluationValue& other) const
{
    return !(*this >= other);
}

bool evaluationValue::operator<=(const evaluationValue& other) const
{
    return !(*this > other);
}

///// Ultimate3TState definitions /////

void Ultimate3TState::init
(
    evaluationValue eval,
    std::vector<std::vector<player>> board, 
    std::vector<player> superBoardResults,
    move bestMove,
    activeBoard aBoard,
    player activePlayer
)
{
    evaluation_ = eval;
    board_ = board;
    superBoardResults_ = superBoardResults;
    bestMove_ = bestMove;
    activeBoard_ = aBoard;
    activePlayer_ = activePlayer;
}

player Ultimate3TState::boardResults(std::vector<player> board) const
{
    // check each possible win combination for x and o
    for( int check = player::x, i = 0; i < 2; check = player::o, i++)
    {
        if (
            // horizontal triplets
            (board[0] == check and board[1] == check and board[2] == check) or
            (board[3] == check and board[4] == check and board[5] == check) or
            (board[6] == check and board[7] == check and board[8] == check) or
            // vertical triplets
            (board[0] == check and board[3] == check and board[6] == check) or
            (board[1] == check and board[4] == check and board[7] == check) or
            (board[2] == check and board[5] == check and board[8] == check) or
            // diagonals
            (board[0] == check and board[4] == check and board[8] == check) or
            (board[2] == check and board[4] == check and board[6] == check)
        ) { return player(check); }
    }
    // if noone has won, is the game still going?
    for (int i = 0; i < 8; i++)
    {
        if (board[i] == player::neither) { return player::neither; }
    }
    // if noone has won, and the game has ended, then it is a draw.
    return player::draw;
}

player Ultimate3TState::utility()
{
    return boardResults(superBoardResults_);
}

Ultimate3TState::Ultimate3TState()
{
    init
    (
        evaluationValue(),
        std::vector<std::vector<player>>(TicTacToeNumberOfSpaces, std::vector<player>(TicTacToeNumberOfSpaces, player::neither)),
        std::vector<player>(TicTacToeNumberOfSpaces, player::neither),
        move(),
        activeBoard::anyBoard,
        player::x
    );
}

Ultimate3TState::~Ultimate3TState() {}

Ultimate3TState::Ultimate3TState(std::bitset<ENCODINGSIZE> binaryEncoding) 
{
    std::bitset<ENCODINGSIZE> copy = binaryEncoding;
    bestMove_ = move(numberBinaryExtraction(8, copy));
    evaluation_ = evaluationValue(player(numberBinaryExtraction(2, copy)), 0);
    activePlayer_ = player(numberBinaryExtraction(2, copy));
    activeBoard_ = activeBoard(numberBinaryExtraction(4, copy));
    superBoardResults_ = std::vector<player>(TicTacToeNumberOfSpaces, player::neither);
    for (int i = 0; i < TicTacToeNumberOfSpaces; i++)
    {
        superBoardResults_[TicTacToeNumberOfSpaces-(i +1)] = player(numberBinaryExtraction(2, copy));
    }
    board_ = std::vector<std::vector<player>>(TicTacToeNumberOfSpaces, std::vector<player>(TicTacToeNumberOfSpaces, player::neither));
    for (int i = 0; i < TicTacToeNumberOfSpaces; i++)
    {
        for (int j = 0; j < TicTacToeNumberOfSpaces; j++)
        {
            board_[TicTacToeNumberOfSpaces-(i +1)][TicTacToeNumberOfSpaces-(j +1)] = player(numberBinaryExtraction(2, copy));
        }
    }
}

Ultimate3TState::Ultimate3TState(Ultimate3TState& source)
{
    init
    (
        source.evaluation_,
        source.board_,
        source.superBoardResults_,
        source.bestMove_,
        source.activeBoard_,
        source.activePlayer_
    );
}

evaluationValue Ultimate3TState::getEvaluation() const { return evaluation_; }

void Ultimate3TState::setEvaluation(evaluationValue newEvaluation) { evaluation_ = newEvaluation; }

std::vector<std::vector<player>> Ultimate3TState::getBoard() const { return board_; }

void Ultimate3TState::setBoard(std::vector<std::vector<player>> newBoard) {
    if ((newBoard.size() != TicTacToeNumberOfSpaces))
    {
        throw std::invalid_argument("Tried to setBoard with invalid board");
    }
    for (int i = 0; i < TicTacToeNumberOfSpaces - 1; i++)
    {
        if (newBoard[i].size() != TicTacToeNumberOfSpaces)
        {
            throw std::invalid_argument("Tried to setBoard with invalid board");
        }
    }
    board_ = newBoard;
}

move Ultimate3TState::getBestMove() const { return bestMove_; }

void Ultimate3TState::setBestMove(move newBestMove) { bestMove_ = newBestMove; }

activeBoard Ultimate3TState::getActiveBoard() const { return activeBoard_; }

void Ultimate3TState::setActiveBoard(activeBoard newActiveBoard) { activeBoard_ = newActiveBoard; }

player Ultimate3TState::getActivePlayer() const { return activePlayer_; }

void Ultimate3TState::setActivePlayer(player newActivePlayer) { activePlayer_ = newActivePlayer; }

player Ultimate3TState::getSpacePlayed(int boardNumber, int spaceNumber) const
{
    if ((boardNumber >= TicTacToeNumberOfSpaces) or (spaceNumber >= TicTacToeNumberOfSpaces))
    {
        throw std::out_of_range("Tried to getSpacePlayed out of board Range");
    }
    return board_[boardNumber][spaceNumber];
}

void Ultimate3TState::setSpacePlayed(int boardNumber, int spaceNumber, player whoPlayed) 
{
    if ((boardNumber >= TicTacToeNumberOfSpaces) or (spaceNumber >= TicTacToeNumberOfSpaces))
    {
        throw std::out_of_range("Tried to setSpacePlayed out of board Range");
    }
    board_[boardNumber][spaceNumber] = whoPlayed;
    // if this move causes a player to win, update the superBoard.
    if (superBoardResults_[boardNumber] == player::neither) 
    { 
        superBoardResults_[boardNumber] = boardResults(board_[boardNumber]);
    }
}

std::vector<move> Ultimate3TState::generateMoves()
{
    std::vector<move> legalMoves;
    if (isTerminalState())
    {
        return legalMoves;
    }
    if (activeBoard_ != activeBoard::anyBoard)
    {
        for (int space = 0; space < TicTacToeNumberOfSpaces; space++)
        {
            if (getSpacePlayed(activeBoard_, space) == player::neither)
            {
                legalMoves.push_back(move(activeBoard_, space));
            }
        }
        if (legalMoves.size() > 0) { return legalMoves; }
    }
    // the active board is any board, or the active board had no legal moves.
    for (int board = 0; board < TicTacToeNumberOfSpaces; board++)
    {
        for (int space = 0; space < TicTacToeNumberOfSpaces; space++)
        {
            if (getSpacePlayed(board, space) == player::neither)
            {
                legalMoves.push_back(move(activeBoard(board), space));
            }
        }
    }
    return legalMoves;
}

Ultimate3TState Ultimate3TState::generateSuccessorState(move playedMove)
{
    Ultimate3TState successor(*this); // create a copy of this State to work from
    if ( // check for legal move
        successor.getSpacePlayed(playedMove.board, playedMove.space) != player::neither
        || (activeBoard_ != playedMove.board && activeBoard_ != activeBoard::anyBoard)
        ) 
    {
        throw std::invalid_argument("Tried to generate seccessor from illegal move");
    }
    successor.setSpacePlayed
    (
        playedMove.board, playedMove.space, // where the move is to be played
        getActivePlayer() // who is playing the move
    );
    successor.setActivePlayer(getActivePlayer() == player::x ? player::o : player::x); // make it the other player's turn.
    // determine if the next board to be played on is full. if it is, then any board can be played on. If not, the board corresponding to the space of the played move must be played on.
    successor.setActiveBoard(activeBoard::anyBoard);
    for (int space = 0; space < 8; space++)
    {
        if (successor.getSpacePlayed(playedMove.space, space) == player::neither)
        {
            successor.setActiveBoard(activeBoard(playedMove.space));
        }
    }
    return successor;
}

bool Ultimate3TState::isTerminalState()
{
    // if the game is still going, utility will return player::neither. otherwise the game has ended somehow.
    return utility() != player::neither;
}

void Ultimate3TState::numberBinaryInsertion(int number, int size, std::bitset<ENCODINGSIZE>& binary) const
{
    // allocate new space for the number
    binary <<= size;
    // turn the number into binary
    std::bitset<ENCODINGSIZE> temp(number);
    // read the new binary into the binary string.
    for (int i = 0; i < size; i++)
    {
        binary.set(i, temp[i]);
    }
}

int Ultimate3TState::numberBinaryExtraction(int size, std::bitset<ENCODINGSIZE>& binary) const
{
    int value = 0;
    for (int i = 0; i < size; i++)
    {
        value += binary[0]*std::pow(2, i);
        binary >>= 1;
    }
    return value;
}

std::bitset<ENCODINGSIZE> Ultimate3TState::toBinary() const
{
    std::bitset<ENCODINGSIZE> binary;

    // iterate over the board and encode it into binary.
    for (int i = 0; i < TicTacToeNumberOfSpaces; i++)
    {
        for (int j = 0; j < TicTacToeNumberOfSpaces; j++)
        {
            numberBinaryInsertion(board_[i][j], 2, binary);
        }
    }

    for (int i = 0; i < TicTacToeNumberOfSpaces; i++)
    {
        numberBinaryInsertion(superBoardResults_[i], 2, binary);
    }
        
    numberBinaryInsertion(activeBoard_, 4, binary);
    numberBinaryInsertion(activePlayer_, 2, binary);
    numberBinaryInsertion(evaluation_.playerToWin, 2, binary);
    numberBinaryInsertion(bestMove_.toBinary(), 8, binary);
    
    return binary;
}