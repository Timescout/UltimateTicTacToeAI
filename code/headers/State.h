/* State.h
Ultimate Tic Tac Toe AI project
Author: Andrew Bergman
8/28/2023

This file defines a game state for ultimate tic tac toe and it's encoding to a file.

*/
#include <stdint.h>
#include <vector>
#include <stdexcept>

// Defines encoding for which board is active. 
enum activeBoard : uint8_t
{
    board0    = 0b0000,
    board1    = 0b0001,
    board2    = 0b0010,
    board3    = 0b0011,
    board4    = 0b0100,
    board5    = 0b0101,
    board6    = 0b0110,
    board7    = 0b0111,
    board8    = 0b1000,
    anyBoard  = 0b1111
};

// a move in this game has two parts the board and space, which each range from 0 to 8 inclusive.
/// @brief Represents a player. Context gives this more meaning, it could mean that this player is winning or that it is this player's turn
enum player : uint8_t
{
    x,
    o,
    draw,
    neither
};

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
    uint8_t toBinary();
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
    evaluationValue(player toWin, int depthToWin);

    ///// Comparison methods /////
    bool const operator==(const evaluationValue& other);
    bool const operator!=(const evaluationValue& other);
    bool const operator>(const evaluationValue& other);
    bool const operator>=(const evaluationValue& other);
    bool const operator<(const evaluationValue& other);
    bool const operator<=(const evaluationValue& other);
};

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
    player boardResults(std::vector<player> board);

public:

    ///// Constructors and destructor /////

    /// @brief Creates a state equivilant to the starting position
    Ultimate3TState();

    /// @brief Destructor.
    ~Ultimate3TState();

    /// @brief Creates a state from a given hex value.
    /// @param binaryEncoding A binary encoding of the State which is transformed into a State object. 
    // warning, not yet implemented.
    Ultimate3TState(std::vector<uint64_t> binaryEncoding);

    /// @brief Copy constructor.
    Ultimate3TState(Ultimate3TState& source);

    ///// Get and set /////

    const evaluationValue getEvaluation();
    void setEvaluation(evaluationValue newEvaluation);

    const std::vector<std::vector<player>> getBoard();
    void setBoard(std::vector<std::vector<player>> newBoard);

    const move getBestMove();
    void setBestMove(move newBestMove);

    const activeBoard getActiveBoard();
    void setActiveBoard(activeBoard newActiveBoard);

    const player getActivePlayer();
    void setActivePlayer(player newActivePlayer);

    const player getSpacePlayed(int boardNumber, int spaceNumber);
    /// @brief Sets a space state, as if a player played their during their turn. Throws an error if the indecies are outside of the possible board values.
    /// @param boardNumber The board to play on.
    /// @param spaceNumber The space to play on.
    /// @param whoPlayed Which player plays this move.
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
    std::vector<u_int64_t> const toBinary();

};