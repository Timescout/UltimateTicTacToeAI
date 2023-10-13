/* State.h
Ultimate Tic Tac Toe AI project
Author: Andrew Bergman
8/28/2023

This file defines a game state for ultimate tic tac toe and it's encoding to a file.

Including:
    active board enum
    player enum
    move struct
    evaluationValue struct
    Ultimate3TState class
*/
#pragma once
#include <stdint.h>
#include <vector>
#include <stdexcept>
#include <bitset>

// Defines encoding for which board is active. 
enum activeBoard : uint8_t
{
    board0   = 0b0000,
    board1   = 0b0001,
    board2   = 0b0010,
    board3   = 0b0011,
    board4   = 0b0100,
    board5   = 0b0101,
    board6   = 0b0110,
    board7   = 0b0111,
    board8   = 0b1000,
    anyBoard = 0b1111
};

// a move in this game has two parts the board and space, which each range from 0 to 8 inclusive.
/// @brief Represents a player. Context gives this more meaning, it could mean that this player is winning or that it is this player's turn
enum player : uint8_t
{
    x       = 0b11,
    o       = 0b10,
    draw    = 0b01,
    neither = 0b00
};

/// @brief Object that holds information about a move. 
struct move
{
private:
    void init(activeBoard moveBoard, uint8_t moveSpace);
public:
    activeBoard board;
    uint8_t space;

    /// @brief default constructor
    move();

    /// @brief Argumented constructor
    /// @param moveBoard The board of this move, Note this cannot be activeBoard::anyBoard which will throw an error if passed.
    /// @param moveSpace The space of this move, must be between 0 and 8 inclusive. Throws an error if outside this range.
    move(activeBoard moveBoard, uint8_t moveSpace);

    /// @brief Construct a move from binary.
    /// @param binary The binary to convert to a move.
    move(uint8_t binary);

    /// @brief Convert this move to a binary representation
    /// @return A binary representation of this move.
    uint8_t toBinary() const;
};

/// @brief In order to ensure more intelligent behavior, our evaluation must include more information than just the game's value. The depth is also stored and attached to the value of the game. A greater depth is always worse than a lesser depth.
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
    evaluationValue(player toWin, int depthToWin);

    ///// Comparison methods /////
    // Used in the minimax algorithm to compare moves.
    bool operator==(const evaluationValue& other) const;
    bool operator!=(const evaluationValue& other) const;
    bool operator>(const evaluationValue& other) const;
    bool operator>=(const evaluationValue& other) const;
    bool operator<(const evaluationValue& other) const;
    bool operator<=(const evaluationValue& other) const;
};

// A number used to define the size needed to encode an Ultimate3TState into Binary.
#define ENCODINGSIZE 196

/// @brief A game state for ultimate tic tac toe. 
class Ultimate3TState
{
private:
    /// @brief The number of Spaces in a normal Tic Tac Toe grid. This is used throughout to check bounds and to prevent magic numbers.
    static const int TicTacToeNumberOfSpaces = 9;

    /// @brief The evaluation of this position. -1 is a win for player O, 1 is a win for player X, and 0 is a draw.
    evaluationValue evaluation_;

    /// @brief The evaluated best move in this state.
    move bestMove_;

    /// @brief The board associated with this state. Keeps track of which player has played in a space.
    std::vector<std::vector<player>> board_;

    /// @brief Stores the ongoing state of who has won subgames. 
    std::vector<player> superBoardResults_;

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
        std::vector<player> superBoardResults,
        move bestMove, 
        activeBoard aBoard, 
        player activePlayer
    );

    /// @brief Gets the result of the board.
    /// @param board The board number to check.
    /// @return The winner of the board, draw if it is a draw, or neither if the game is still ongoing. 
    player boardResults(std::vector<player> board) const;

    /// @brief Used for encoding a number into a binary string. The number will be appended to the beggining of the bitset
    /// @param number The number to be encoded
    /// @param size the number will take in the binary string
    /// @param binary The binary string, the number will be appended to the end.
    void numberBinaryInsertion(int number, int size, std::bitset<ENCODINGSIZE>& binary) const;

    /// @brief Used to extract numbers from a bitset.
    /// @param start Where to start reading.
    /// @param end Where to end(exclusive) reading.
    /// @param binary The bitset to extract from.
    /// @return The number extracted.
    int numberBinaryExtraction(int start, int end, std::bitset<ENCODINGSIZE>& binary) const;

public:

    ///// Constructors and destructor /////

    /// @brief Creates a state equivilant to the starting position
    Ultimate3TState();

    /// @brief Destructor.
    ~Ultimate3TState();

    /// @brief Creates a state from a given hex value.
    /// @param binaryEncoding A binary encoding of the State which is transformed into a State object. 
    // warning, not yet implemented.
    Ultimate3TState(std::bitset<ENCODINGSIZE>);

    /// @brief Copy constructor.
    Ultimate3TState(Ultimate3TState& source);

    ///// Get and set /////

    evaluationValue getEvaluation() const;
    void setEvaluation(evaluationValue newEvaluation);

    std::vector<std::vector<player>> getBoard() const;
    void setBoard(std::vector<std::vector<player>> newBoard);

    move getBestMove() const;
    void setBestMove(move newBestMove);

    activeBoard getActiveBoard() const;
    void setActiveBoard(activeBoard newActiveBoard);

    player getActivePlayer() const;
    void setActivePlayer(player newActivePlayer);

    player getSpacePlayed(int boardNumber, int spaceNumber) const;
    /// @brief Sets a space state, as if a player played their during their turn. Throws an error if the indecies are outside of the possible board values.
    /// @param boardNumber The board to play on.
    /// @param spaceNumber The space to play on.
    /// @param whoPlayed Which player plays this move. This can be any player enum, including niether and draw.
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

    /// @brief Checks the utility of the game
    /// @param checkBoard A vector of player enums that holds info about which players have played in which spaces
    /// @return The result of the game. Can be x, o, a draw, or niether. If neither, the game is still being played.
    player utility();

    /// @brief Transforms this state into a binary string. 
    /// @return A binary version of this State.
    // Warning, not yet implemented.
    /// @warning The depth value of the evaluation_ is not saved and so this function is lossy.
    std::bitset<ENCODINGSIZE> toBinary() const;
};