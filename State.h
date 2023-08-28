/* State.h
Ultimate Tic Tac Toe AI project
Author: Andrew Bergman
8/28/2023

This file defines a game state for ultimate tic tac toe and it's encoding to a file.

*/
#include <stdint.h>
#include <vector>

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

// Defines encoding for evaluation scores
enum evaluation
{
    draw = 0x00, // decimal 0
    xWin = 0x01, // decimal 1
    oWin = 0x10 // decimal 2
};

// Defines encoding for which player's turn it is.
enum toPlay
{
    o = 0,
    x = 1
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

// defines encoding for the state of a space.
enum spaceState
{
    empty = 0,
    xPlayed = 0x11,
    oPlayed = 0x10
};



/// @brief A game state for ultimate tic tac toe. 
class Ultimate3TState
{
private:
    /// @brief The evaluation of this position. -1 is a win for player O, 1 is a win for player X, and 0 is a draw.
    evaluation evaluation_;

    /// @brief The board associated with this state.
    std::vector<std::vector<spaceState>> board_;

    /// @brief The evaluated best move in this state.
    move bestMove_;

    /// @brief The active board which can be played on this turn.
    /// @note -1 means that any board may be played on, and may be a result of the game just starting.
    activeBoard activeBoard_; 

    /// @brief Who the active player is, true is player X and false is player O.
    toPlay activePlayer_;

    /// @brief initializes variables so they can be filled in with the correct values.
    void init();
public:

    ///// Constructors and destructor /////

    /// @brief Creates a state equivilant to the starting position
    Ultimate3TState();

    /// @brief Destructor.
    ~Ultimate3TState();

    /// @brief Creates a state from a given hex value.
    /// @param binaryEncoding A binary encoding of the State which is transformed into a State object. 
    Ultimate3TState(std::vector<uint64_t> binaryEncoding);

    ///// Get and set /////

    evaluation getEvaluation();
    void setEvaluation(evaluation newEvaluation);

    std::vector<std::vector<spaceState>> getBoard();
    void setBoard(std::vector<std::vector<spaceState>> newBoard);

    move getBestMove();
    void setBestMove(move newBestMove);

    activeBoard getActiveBoard();
    void setActiveBoard(activeBoard newActiveBoard);

    toPlay getActivePlayer();
    void setActivePlayer(toPlay newActivePlayer);

    spaceState getSpaceState(int boardNumber, int spaceNumber);
    void setSpaceState(int boardNumber, int spaceNumber, spaceState newSpaceState);

    ///// Functions /////

    /// @brief Generate a vector of legal moves in this state.
    /// @return A vector of legal moves
    std::vector<move> generateMoves();

    /// @brief Generates a state where the given move was played in this state.
    /// @param playedMove the move to be played
    /// @return A State where the game has progressed after the given move was played.
    Ultimate3TState generateSuccessorState(move playedMove);

    /// @brief Checks if this state is a terminal state. This can be because a player won, or there are no remaining legal moves.
    /// @return true if the state is terminal, false otherwise.
    bool isTerminalState();
};