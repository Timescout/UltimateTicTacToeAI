#include "State.h"

///// move struct definitions /////

move::move()
:   board(0),
    space(0)
{}

move::move(uint8_t board, uint8_t space) 
:   board(board), 
    space(space)
{}

move::move(uint8_t binary)
{
    board = binary >> 4; // select the top bits
    space = binary % 0x10000; // select the bottom bits
}

uint8_t move::toBinary() 
{
    return (board << 4) + space;
}

///// Ultimate3TState definitions /////

void Ultimate3TState::init()
{
    evaluation_ = evaluation::draw;
    board_ = std::vector<std::vector<spaceState>>(8, std::vector<spaceState>(8, spaceState::empty));
    activeBoard_ = activeBoard::anyBoard; // game just started, so any board may be played on.
    activePlayer_ = toPlay::x;
}

Ultimate3TState::Ultimate3TState()
{
    init();
}

Ultimate3TState::~Ultimate3TState() {}

Ultimate3TState::Ultimate3TState(std::vector<uint64_t> binaryEncoding) {}

evaluation Ultimate3TState::getEvaluation() { return evaluation_; }

void Ultimate3TState::setEvaluation(evaluation newEvaluation) { evaluation_ = newEvaluation; }

std::vector<std::vector<spaceState>> Ultimate3TState::getBoard() { return board_; }

void Ultimate3TState::setBoard(std::vector<std::vector<spaceState>> newBoard) { board_ = newBoard; }

move Ultimate3TState::getBestMove() { return bestMove_; }

void Ultimate3TState::setBestMove(move newBestMove) { bestMove_ = newBestMove; }

activeBoard Ultimate3TState::getActiveBoard() { return activeBoard_; }

void Ultimate3TState::setActiveBoard(activeBoard newActiveBoard) { activeBoard_ = newActiveBoard; }

toPlay Ultimate3TState::getActivePlayer() { return activePlayer_; }

void Ultimate3TState::setActivePlayer(toPlay newActivePlayer) { activePlayer_ = newActivePlayer; }

spaceState Ultimate3TState::getSpaceState(int boardNumber, int spaceNumber) { return board_[boardNumber][spaceNumber]; }

void Ultimate3TState::setSpaceState(int boardNumber, int spaceNumber, spaceState newSpaceState) { board_[boardNumber][spaceNumber] = newSpaceState; }

std::vector<move> Ultimate3TState::generateMoves()
{
    std::vector<move> legalMoves;
    if (activeBoard_ != activeBoard::anyBoard)
    {
        for (int space = 0; space < 8; space++)
        {
            if (getSpaceState(activeBoard_, space) == spaceState::empty)
            {
                legalMoves.push_back(move(activeBoard_, space));
            }
        }
        if (legalMoves.size() > 0) { return legalMoves; }
    }
    // the active board is any board, or the active board had no legal moves.
    for (int board = 0; board < 8; board++)
    {
        for (int space = 0; space < 8; space++)
        {
            if (getSpaceState(board, space) == spaceState::empty)
            {
                legalMoves.push_back(move(board, space));
            }
        }
    }
    return legalMoves;
}

Ultimate3TState Ultimate3TState::generateSuccessorState(move playedMove)
{
    
}