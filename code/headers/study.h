
#include "State.h"
#include <vector>

evaluation study(Ultimate3TState gameState)
{
    if (gameState.isTerminalState())
    {

    }
    std::vector<move> legalMoves = gameState.generateMoves();
    gameState gameResult = gameState.getEvaluation();
    for (std::vector<move>::iterator moveIterator = legalMoves.begin(); moveIterator != legalMoves.end(); moveIterator++)
    {
        Ultimate3TState successor = gameState.generateSuccessorState(*moveIterator);
        // search for successor in table
        //    gameResult = table[successor].getEvaluation();
        // else 
        {
            gameResult = study(successor);
            if (gameResult == evaluation::oWin &&
                gameState.getActivePlayer() == toPlay::o &&
                )
                {

                }
        }
    }
}