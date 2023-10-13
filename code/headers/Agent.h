/* Agent.h
Ultimate Tic Tac Toe AI project
Andrew Bergman
9/11/23

This file will contain the game code of the AI Agent who plays the game and the trainer class that creates the move file the the AI will read from.

*/
#pragma once
#include "State.h"
#include <set>

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

    std::set<std::bitset<ENCODINGSIZE>, EncodingCompare> transpositionTable_;

    Ultimate3TState startState_;

    void init();

public:
    AgentTrainer();
    AgentTrainer(const Ultimate3TState startState);

    evaluationValue minimax(Ultimate3TState& state, int depth = 0);

    void writeToFile();
};

