/* Agent.h
Ultimate Tic Tac Toe AI project
Andrew Bergman
9/11/23

This file will contain the game code of the AI Agent who plays the game and the trainer class that creates the move file the the AI will read from.

*/
#pragma once
#include "State.h"
#include <map>

/// @brief Used to compare two bitsets of ENCODINGSIZE bits. This function is used to create a set of encodings.
struct EncodingCompare
{
    /// @brief Equivilant to a < b
    /// @param a First bitset to compare.
    /// @param b Second bitset to compare.
    bool operator()(std::bitset<ENCODINGSIZE> a, std::bitset<ENCODINGSIZE> b) const;
};

class AgentTrainer
{
private:

    /// @brief Transposition table that holds states that have already been searched. The value mapped to is a pair, the best move in the state and the evaluation of the best move. This is because to create the encoding for a state we already need to know the best move which defeats the purpose of the transposition table and the depth is not encoded at all. In order to preserve depth values, evaluation values are needed.
    std::map<std::bitset<ENCODINGSIZE>, std::pair<evaluationValue, move>, EncodingCompare> transpositionTable_;

    /// @brief The Stream that the transposition table will be written to when writeToOutput() is called.
    std::ostream* outputStream_;

    /// @brief Keeps track of the number of times generateMoves() is called during minimax. 
    unsigned int statesExpanded_;

    void init(std::ostream& outputStream);

public:
    /// @brief Creates a default AgentTrainer. Default values are the starting U3T state and std::cout.
    AgentTrainer();

    /// @brief Creates an AgentTrainer which starts on the given state and outputs to the given ostream.
    /// @param outputStream The ostream to output to.
    AgentTrainer(std::ostream& outputStream);

    /// @brief Deconstructor
    ~AgentTrainer();

    /// @brief Run the minimax algorithm on the given state. Recursivly searches the State tree to find the best move and the evaluation of each state that can be reached from the given state. The move and evaluation will be in the transposition table.
    /// @param state The state to Start the search from.
    /// @return The evaluation of the state.
    evaluationValue minimax(Ultimate3TState& state);

    /// @brief Write the output of the transposition table to outputStream_. 
    void writeToOutput();

    /// @brief Resets the transposition table for a new state. This is so that running minimax multiple times does not cross contaminate runs.
    void resetTranspositionTable();

    /// @brief Gets the number of states expanded during the minimax algorithm.
    unsigned int getStatesExpanded();

    /// @brief Resets the number of states expanded during the minimax algorithm to 0. 
    void resetStatesExpanded();
};

