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
}
using namespace AgentTrainerTestFunctions;

TEST(AgentTrainerTests, Minimax_TerminalState_DoesNotExpandMoreStates) 
{
    std::stringstream outputStream;
    AgentTrainer trainer(outputStream);
    Ultimate3TState state = createTerminalState();

    trainer.minimax(state);
    trainer.writeToOutput();
    std::string output;
    outputStream >> output;
    
    EXPECT_EQ(outputStream.str().size(), ENCODINGSIZE+1); // test that there is only one entry in the output.
    EXPECT_EQ(state.toBinary().to_string(), output);
}