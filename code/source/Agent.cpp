#include <Agent.h>

evaluationValue AgentTrainer::minimax(Ultimate3TState& state, int depth)
{
    if (state.isTerminalState())
    {
        state.setEvaluation(evaluationValue(state.utility(), depth));
    }

    state.setEvaluation(state.getActivePlayer() == player::x ? evaluationValue(player::o, 0) : evaluationValue(player::x, 0));
    std::vector<move> actions = state.generateMoves();
    for (std::vector<move>::iterator action = actions.begin(); action != actions.end(); action++)
    {
        Ultimate3TState nextState = state.generateSuccessorState(*action);
        minimax(nextState, depth + 1);
        if (!(state.getActivePlayer() == player::x xor state.getEvaluation() > nextState.getEvaluation()))
        {
            state.setBestMove(*action);
            state.setEvaluation(nextState.getEvaluation());
        }
    }

    return evaluationValue();
}

bool EncodingCompare::operator()(std::bitset<ENCODINGSIZE> a, std::bitset<ENCODINGSIZE> b) const
{
    for (int i = ENCODINGSIZE; i > 0; i--)
    {
        if (a[i-1] xor b[i-1])
        {
            return a[i-1] < b[i-1];
        }
    }
    // If all the bits match, Then the
    return false;
}