/* State.h
Ultimate Tic Tac Toe AI project
Author: Andrew Bergman
8/28/2023

This file defines a game state for ultimate tic tac toe and it's encoding to a file.

*/
#include <stdint.h>
#include <vector>
#include <stdexcept>

// a move in this game has two parts the board and space, which each range from 0 to 8 inclusive.
struct move
{
    uint8_t board;
    uint8_t space;

    /// @brief default constructor
    move();

    /// @brief Argumented constructor
    /// @param board The board of this move
    /// @param space The space of this move
    move(uint8_t board, uint8_t space);

    /// @brief Construct a move from binary.
    /// @param binary The binary to convert to a move.
    move(uint8_t binary);

    /// @brief Convert this move to a binary representation
    /// @return A binary representation of this move.
    uint8_t toBinary();
};

/// @brief Represents a player. Context gives this more meaning, it could mean that this player is winning or that it is this player's turn
enum player 
{
    x,
    o,
    draw,
    neither
};

/// @brief In order to ensure more intelligent behavior, our evaluation must include more information than just the game's value.
struct evaluationValue
{
private:
    void init(player toWin, int depthToWin);
public:
    /// @brief What the outcome of the game will be, assuming that both players play optimaly
    player playerToWin;

    /// @brief By giving the evaluationValue a depth, we can ensure Tim player for as long as possible when losing and as short as possible when winning.
    int depth;

    evaluationValue();

    bool operator>(const evaluationValue& other);
};

// Defines encoding for which board is active.
enum activeBoard
{
    board0,
    board1,
    board2,
    board3,
    board4,
    board5,
    board6,
    board7,
    board8,
    anyBoard = 0x1111
};

/// @brief A game state for ultimate tic tac toe. 
class Ultimate3TState
{
private:
    /// @brief The evaluation of this position. -1 is a win for player O, 1 is a win for player X, and 0 is a draw.
    evaluationValue evaluation_;

    /// @brief The board associated with this state. Keeps track of which player has played in a space.
    std::vector<std::vector<player>> board_;

    /// @brief The evaluated best move in this state.
    move bestMove_;

    /// @brief The active board which can be played on this turn.
    /// @note -1 means that any board may be played on, and may be a result of the game just starting.
    activeBoard activeBoard_; 

    /// @brief Who the active player is, true is player X and false is player O.
    player activePlayer_;

    /// @brief initializes variables so they can be filled in with the correct values.
    void init
    (
        evaluationValue eval, 
        std::vector<std::vector<player>> board, 
        move bestMove, 
        activeBoard aBoard, 
        player activePlayer
    );

    /// @brief Checks the State of the game
    /// @param checkBoard A vector of player enums that holds info about which players have played in which spaces
    /// @return The player who will win the game. Can be x, o, a draw, or niether. If neither, the game is still being played.
    player stateCheck(std::vector<player> checkBoard);

public:

    ///// Constructors and destructor /////

    /// @brief Creates a state equivilant to the starting position
    Ultimate3TState();

    /// @brief Destructor.
    ~Ultimate3TState();

    /// @brief Creates a state from a given hex value.
    /// @param binaryEncoding A binary encoding of the State which is transformed into a State object. 
    Ultimate3TState(std::vector<uint64_t> binaryEncoding);

    /// @brief Copy constructor.
    Ultimate3TState(Ultimate3TState& source);

    ///// Get and set /////

    evaluationValue getEvaluation();
    void setEvaluation(evaluationValue newEvaluation);

    std::vector<std::vector<player>> getBoard();
    void setBoard(std::vector<std::vector<player>> newBoard);

    move getBestMove();
    void setBestMove(move newBestMove);

    activeBoard getActiveBoard();
    void setActiveBoard(activeBoard newActiveBoard);

    player getActivePlayer();
    void setActivePlayer(player newActivePlayer);

    player getSpacePlayed(int boardNumber, int spaceNumber);
    void setSpacePlayed(int boardNumber, int spaceNumber, player whoPlayed);

    ///// Functions /////

    /// @brief Generate a vector of legal moves in this state.
    /// @return A vector of legal moves
    std::vector<move> generateMoves();

    /// @brief Generates a state where the given move was played in this state.
    /// @param playedMove the move to be played. Throws an error if playedMove is not legal
    /// @return A State where the game has progressed after the given move was played.
    Ultimate3TState generateSuccessorState(move playedMove);

    /// @brief Checks if this state is a terminal state. This can be because a player won, or there are no remaining legal moves.
    /// @return true if the state is terminal, false otherwise.
    bool isTerminalState();

};