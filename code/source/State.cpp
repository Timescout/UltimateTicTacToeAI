#include "State.h"
#include <stdexcept>

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

uint8_t move::toBinary() 
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

bool const evaluationValue::operator==(const evaluationValue& other)
{
    return playerToWin == other.playerToWin && depth == other.depth;
}

bool const evaluationValue::operator!=(const evaluationValue& other)
{
    return !(*this == other);
}

bool const evaluationValue::operator>(const evaluationValue& other)
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

bool const evaluationValue::operator>=(const evaluationValue& other)
{
    return *this > other || *this == other;
}

bool const evaluationValue::operator<(const evaluationValue& other)
{
    return !(*this >= other);
}

bool const evaluationValue::operator<=(const evaluationValue& other)
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

player Ultimate3TState::boardResults(std::vector<player> board) 
{
    // check each possible win combination for x and o
    if (
        // horizontal triplets
        (board[0] == player::x and board[1] == player::x and board[2] == player::x) or
        (board[3] == player::x and board[4] == player::x and board[5] == player::x) or
        (board[6] == player::x and board[7] == player::x and board[8] == player::x) or
        // vertical triplets
        (board[0] == player::x and board[3] == player::x and board[6] == player::x) or
        (board[1] == player::x and board[4] == player::x and board[7] == player::x) or
        (board[2] == player::x and board[5] == player::x and board[8] == player::x) or
        // diagonals
        (board[0] == player::x and board[4] == player::x and board[8] == player::x) or
        (board[2] == player::x and board[4] == player::x and board[6] == player::x)
    ) { return player::x; }
    if (
        // horizontal triplets
        (board[0] == player::o and board[1] == player::o and board[2] == player::o) or
        (board[3] == player::o and board[4] == player::o and board[5] == player::o) or
        (board[6] == player::o and board[7] == player::o and board[8] == player::o) or
        // vertical triplets
        (board[0] == player::o and board[3] == player::o and board[6] == player::o) or
        (board[1] == player::o and board[4] == player::o and board[7] == player::o) or
        (board[2] == player::o and board[5] == player::o and board[8] == player::o) or
        // diagonals
        (board[0] == player::o and board[4] == player::o and board[8] == player::o) or
        (board[2] == player::o and board[4] == player::o and board[6] == player::o)
    ) { return player::o; }
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

Ultimate3TState::Ultimate3TState(std::vector<uint64_t> binaryEncoding) {}

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

const evaluationValue Ultimate3TState::getEvaluation() { return evaluation_; }

void Ultimate3TState::setEvaluation(evaluationValue newEvaluation) { evaluation_ = newEvaluation; }

const std::vector<std::vector<player>> Ultimate3TState::getBoard() { return board_; }

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

const move Ultimate3TState::getBestMove() { return bestMove_; }

void Ultimate3TState::setBestMove(move newBestMove) { bestMove_ = newBestMove; }

const activeBoard Ultimate3TState::getActiveBoard() { return activeBoard_; }

void Ultimate3TState::setActiveBoard(activeBoard newActiveBoard) { activeBoard_ = newActiveBoard; }

const player Ultimate3TState::getActivePlayer() { return activePlayer_; }

void Ultimate3TState::setActivePlayer(player newActivePlayer) { activePlayer_ = newActivePlayer; }

const player Ultimate3TState::getSpacePlayed(int boardNumber, int spaceNumber) 
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
        successor.getActivePlayer() ? player::x : player::o // who is playing the move
    );
    successor.setActivePlayer(successor.getActivePlayer() ? player::o : player::x); // make it the other player's turn.
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