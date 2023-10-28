/* Andrew Bergman
10-15-23
Tests for AgnetTrainer class which includes the minimax function and file output. 
*/
#include "gtest/gtest.h"
#include "Agent.h"
#include <sstream>

namespace AgentTrainerTestFunctions
{
    Ultimate3TState createTerminalState()
    {
        Ultimate3TState state;
        // play in the top row for all top row boards to create a terminal state.
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                state.setSpacePlayed(i, j, player::x);
            }
        }
        return state;
    }

    // Creates a state where the players play a "normal" game of tic tac toe
    Ultimate3TState createSmallGame()
    {
        Ultimate3TState state;

        for (int i = 0; i < 9; i++)
        {
            state.setSpacePlayed(0, i, draw);
            state.setSpacePlayed(1, i, draw);
            state.setSpacePlayed(2, i, x);
            state.setSpacePlayed(3, i, draw);
            state.setSpacePlayed(4, i, draw);
            state.setSpacePlayed(5, i, x);
            state.setSpacePlayed(6, i, o);
            state.setSpacePlayed(7, i, o);
        }

        return state;
    }
}
using namespace AgentTrainerTestFunctions;

TEST(AgentTrainerTests, Minimax_TerminalState_DoesNotExpandMoreStates) 
{
    std::stringstream outputStream;
    AgentTrainer trainer(outputStream);
    Ultimate3TState state = createTerminalState();
    state.setEvaluation(evaluationValue(x, 0));

    trainer.minimax(state);
    trainer.writeToOutput();
    std::string output = outputStream.str();
    
    EXPECT_EQ(outputStream.str().size(), ENCODINGSIZE+1); // test that there is only one entry in the output. size should be encodingsize + 1 since there should be a terminating newline character at the end of the encoding.
    EXPECT_EQ(state.toBinary().to_string() + "\n", output);
}

TEST(AgentTrainerTests, MinimaxTranspositionTable_SimpleLineTree_ExpandsCorrectNumberOfStates)
{
    std::stringstream outputStream;
    AgentTrainer trainer(outputStream);
    Ultimate3TState state = createSmallGame();
    for (int i = 3; i < 9; i++)
    {
        state.setSpacePlayed(8, i, draw);
    }

    trainer.minimax(state);
    trainer.writeToOutput();
    std::string output = outputStream.str();

    EXPECT_EQ(trainer.getStatesExpanded(), 10);
    EXPECT_EQ(output.size(), (ENCODINGSIZE+1) * 13);
}