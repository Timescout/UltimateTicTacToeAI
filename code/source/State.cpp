#include "State.h"
#include <stdexcept>

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

    // determine the value of this state.
    switch (playerToWin)
    {
    case player::x:
        value = 1000;
        value -= depth;
        break;
    case player::o:
        value = -1000;
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
        othersValue = 1000;
        othersValue -= depth;
        break;
    case player::o:
        othersValue = -1000;
        othersValue += depth;
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
    move bestMove,
    activeBoard aBoard,
    player activePlayer
)
{
    evaluation_ = eval;
    board_ = board;
    bestMove_ = bestMove;
    activeBoard_ = aBoard;
    activePlayer_ = activePlayer;
}

player Ultimate3TState::utility()
{
    // Look at the state in each subBoard, then look at the state in the superBoard.
    std::vector<player> littleBoardUtilities(9); 
    for (int i = 0; i < 8; i++)
    {
        // check each possible win combination for x and o
        if (
            board_[i][0] xor board_[i][3] xor board_[i][6] xor player::x == 0 or
            board_[i][0] xor board_[i][1] xor board_[i][2] xor player::x == 0 or
            board_[i][0] xor board_[i][4] xor board_[i][8] xor player::x == 0 or
            board_[i][3] xor board_[i][4] xor board_[i][8] xor player::x == 0 or
            board_[i][6] xor board_[i][7] xor board_[i][8] xor player::x == 0 or
            board_[i][1] xor board_[i][4] xor board_[i][7] xor player::x == 0 or
            board_[i][2] xor board_[i][5] xor board_[i][8] xor player::x == 0 or
            board_[i][2] xor board_[i][4] xor board_[i][6] xor player::x == 0
        ) { littleBoardUtilities[i] = player::x; }
        if (
            board_[i][0] xor board_[i][3] xor board_[i][6] xor player::o == 0 or
            board_[i][0] xor board_[i][1] xor board_[i][2] xor player::o == 0 or
            board_[i][0] xor board_[i][4] xor board_[i][8] xor player::o == 0 or
            board_[i][3] xor board_[i][4] xor board_[i][8] xor player::o == 0 or
            board_[i][6] xor board_[i][7] xor board_[i][8] xor player::o == 0 or
            board_[i][1] xor board_[i][4] xor board_[i][7] xor player::o == 0 or
            board_[i][2] xor board_[i][5] xor board_[i][8] xor player::o == 0 or
            board_[i][2] xor board_[i][4] xor board_[i][6] xor player::o == 0
        ) { littleBoardUtilities[i] = player::o; }
        // if noone has won, is the game still going?
        for (int i = 0; i < 8; i++)
        {
            if (board_[i][i] == player::neither) { littleBoardUtilities[i] = player::neither; }
        }
        // if noone has won, and the game has ended, then it is a draw.
        littleBoardUtilities[i] = player::draw;
    }

    // check each possible win combination for x and o
    if (
        littleBoardUtilities[0] xor littleBoardUtilities[3] xor littleBoardUtilities[6] xor player::x == 0 or
        littleBoardUtilities[0] xor littleBoardUtilities[1] xor littleBoardUtilities[2] xor player::x == 0 or
        littleBoardUtilities[0] xor littleBoardUtilities[4] xor littleBoardUtilities[8] xor player::x == 0 or
        littleBoardUtilities[3] xor littleBoardUtilities[4] xor littleBoardUtilities[8] xor player::x == 0 or
        littleBoardUtilities[6] xor littleBoardUtilities[7] xor littleBoardUtilities[8] xor player::x == 0 or
        littleBoardUtilities[1] xor littleBoardUtilities[4] xor littleBoardUtilities[7] xor player::x == 0 or
        littleBoardUtilities[2] xor littleBoardUtilities[5] xor littleBoardUtilities[8] xor player::x == 0 or
        littleBoardUtilities[2] xor littleBoardUtilities[4] xor littleBoardUtilities[6] xor player::x == 0
    ) { return player::x; }
    if (
        littleBoardUtilities[0] xor littleBoardUtilities[3] xor littleBoardUtilities[6] xor player::o == 0 or
        littleBoardUtilities[0] xor littleBoardUtilities[1] xor littleBoardUtilities[2] xor player::o == 0 or
        littleBoardUtilities[0] xor littleBoardUtilities[4] xor littleBoardUtilities[8] xor player::o == 0 or
        littleBoardUtilities[3] xor littleBoardUtilities[4] xor littleBoardUtilities[8] xor player::o == 0 or
        littleBoardUtilities[6] xor littleBoardUtilities[7] xor littleBoardUtilities[8] xor player::o == 0 or
        littleBoardUtilities[1] xor littleBoardUtilities[4] xor littleBoardUtilities[7] xor player::o == 0 or
        littleBoardUtilities[2] xor littleBoardUtilities[5] xor littleBoardUtilities[8] xor player::o == 0 or
        littleBoardUtilities[2] xor littleBoardUtilities[4] xor littleBoardUtilities[6] xor player::o == 0
    ) { return player::o; }
    // if noone has won, is the game still going?
    for (int i = 0; i < 8; i++)
    {
        if (littleBoardUtilities[i] == player::neither) { return player::neither; }
    }
    // if noone has won, and the game has ended, then it is a draw.
    return player::draw;
}

Ultimate3TState::Ultimate3TState()
{
    init
    (
        evaluationValue(),
        std::vector<std::vector<player>>(8, std::vector<player>(8, player::neither)),
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
        source.getEvaluation(),
        source.getBoard(),
        source.getBestMove(),
        source.getActiveBoard(),
        source.getActivePlayer()
    );
}

evaluationValue Ultimate3TState::getEvaluation() { return evaluation_; }

void Ultimate3TState::setEvaluation(evaluationValue newEvaluation) { evaluation_ = newEvaluation; }

std::vector<std::vector<player>> Ultimate3TState::getBoard() { return board_; }

void Ultimate3TState::setBoard(std::vector<std::vector<player>> newBoard) { board_ = newBoard; }

move Ultimate3TState::getBestMove() { return bestMove_; }

void Ultimate3TState::setBestMove(move newBestMove) { bestMove_ = newBestMove; }

activeBoard Ultimate3TState::getActiveBoard() { return activeBoard_; }

void Ultimate3TState::setActiveBoard(activeBoard newActiveBoard) { activeBoard_ = newActiveBoard; }

player Ultimate3TState::getActivePlayer() { return activePlayer_; }

void Ultimate3TState::setActivePlayer(player newActivePlayer) { activePlayer_ = newActivePlayer; }

player Ultimate3TState::getSpacePlayed(int boardNumber, int spaceNumber) { return board_[boardNumber][spaceNumber]; }

void Ultimate3TState::setSpacePlayed(int boardNumber, int spaceNumber, player whoPlayed) { board_[boardNumber][spaceNumber] = whoPlayed; }

std::vector<move> Ultimate3TState::generateMoves()
{
    std::vector<move> legalMoves;
    if (activeBoard_ != activeBoard::anyBoard)
    {
        for (int space = 0; space < 8; space++)
        {
            if (getSpacePlayed(activeBoard_, space) == player::neither)
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
            if (getSpacePlayed(board, space) == player::neither)
            {
                legalMoves.push_back(move(board, space));
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