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

///// evaluationValue definitions /////

void evaluationValue::init(player toWin, int depthToWin)
{
    playerToWin = toWin;
    depth = depthToWin;
}

evaluationValue::evaluationValue()
{
    init(player::draw, 0);
}

bool evaluationValue::operator>(const evaluationValue& other)
{
    if (playerToWin == x && other.playerToWin == o)
    {
        return true;
    }
    return false;
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

player Ultimate3TState::utility(std::vector<player> checkBoard)
{
    // check each possible win combination for x and o
    if (
        checkBoard[0] xor checkBoard[3] xor checkBoard[6] xor player::x == 0 or
        checkBoard[0] xor checkBoard[1] xor checkBoard[2] xor player::x == 0 or
        checkBoard[0] xor checkBoard[4] xor checkBoard[8] xor player::x == 0 or
        checkBoard[3] xor checkBoard[4] xor checkBoard[8] xor player::x == 0 or
        checkBoard[6] xor checkBoard[7] xor checkBoard[8] xor player::x == 0 or
        checkBoard[1] xor checkBoard[4] xor checkBoard[7] xor player::x == 0 or
        checkBoard[2] xor checkBoard[5] xor checkBoard[8] xor player::x == 0 or
        checkBoard[2] xor checkBoard[4] xor checkBoard[6] xor player::x == 0
    ) { return player::x; }
    if (
        checkBoard[0] xor checkBoard[3] xor checkBoard[6] xor player::o == 0 or
        checkBoard[0] xor checkBoard[1] xor checkBoard[2] xor player::o == 0 or
        checkBoard[0] xor checkBoard[4] xor checkBoard[8] xor player::o == 0 or
        checkBoard[3] xor checkBoard[4] xor checkBoard[8] xor player::o == 0 or
        checkBoard[6] xor checkBoard[7] xor checkBoard[8] xor player::o == 0 or
        checkBoard[1] xor checkBoard[4] xor checkBoard[7] xor player::o == 0 or
        checkBoard[2] xor checkBoard[5] xor checkBoard[8] xor player::o == 0 or
        checkBoard[2] xor checkBoard[4] xor checkBoard[6] xor player::o == 0
    ) { return player::o; }
    // if noone has won, is the game still going?
    for (int i = 0; i < 8; i++)
    {
        if (checkBoard[i] == player::neither) { return player::neither; }
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
    std::vector<player> superBoard(9, player::neither);
    for (int i = 0; i < 8; i++)
    {
        superBoard[i] = utility(board_[i]);
    }
    // if the game is still going, utility will return player::neither. otherwise the game has ended somehow.
    return utility(superBoard) != player::neither;
}