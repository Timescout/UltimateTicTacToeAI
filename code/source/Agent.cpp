#include <Agent.h>
#include <iostream>

///// AgentTrainer definitions /////

void AgentTrainer::init(std::ostream& outputStream)
{
    outputStream_ = &outputStream;
    transpositionTable_ = std::map<std::bitset<ENCODINGSIZE>, std::pair<evaluationValue, move>, EncodingCompare>();
}

AgentTrainer::AgentTrainer()
{
    init(std::cout);
}

AgentTrainer::AgentTrainer(std::ostream& outputStream)
{
    init(outputStream);
}

AgentTrainer::~AgentTrainer()
{
    outputStream_ = nullptr;
}

// Not allowed to change the evaluationValue or bestMove of state, so that we can find it in the transposition table later without first finding those values.
evaluationValue AgentTrainer::minimax(Ultimate3TState& state)
{
    // check if the state is in the transposition table
    auto transpositionTableEntry = transpositionTable_.find(state.toBinary());
    if (transpositionTableEntry != transpositionTable_.end())
    {
        evaluationValue value = transpositionTableEntry->second.first;
        value.depth += 1;
        return value; // Return the evaluationValue in the transposition table.
    }

    if (state.isTerminalState())
    {
        evaluationValue value(state.utility(), 0);
        // put state into the transposition table
        transpositionTable_.insert(std::pair<std::bitset<ENCODINGSIZE>, std::pair<evaluationValue, move>>(state.toBinary(), std::pair<evaluationValue, move>(value, move())));
        return value;
    }

    evaluationValue value = state.getActivePlayer() == player::x ? evaluationValue(player::o, 0) : evaluationValue(player::x, 0);
    evaluationValue nextStateValue;
    std::vector<move> actions = state.generateMoves();
    move bestMove = actions[0];
    for (std::vector<move>::iterator action = actions.begin(); action != actions.end(); action++)
    {
        Ultimate3TState nextState = state.generateSuccessorState(*action);
        nextStateValue = minimax(nextState);
        if (!(state.getActivePlayer() == player::x xor value > nextStateValue))
        {
            bestMove = *action;
            value = nextStateValue;
        }
    }
    // insert into transposition table
    transpositionTable_.insert(std::pair<std::bitset<ENCODINGSIZE>, std::pair<evaluationValue, move>>(state.toBinary(), std::pair<evaluationValue, move>(value, bestMove)));
    // because there was a move to get to this state, we must increase the depth by one here.
    value.depth += 1;
    return value;
}

void AgentTrainer::writeToOutput()
{
    for (auto state = transpositionTable_.begin(); state != transpositionTable_.end(); state++)
    {
        Ultimate3TState temp(state->first);
        temp.setBestMove(state->second.second);
        temp.setEvaluation(state->second.first);
        *outputStream_ << temp.toBinary() << "\n";
    }
}

void AgentTrainer::resetTranspositionTable()
{
    transpositionTable_ = std::map<std::bitset<ENCODINGSIZE>, std::pair<evaluationValue, move>, EncodingCompare>();
}

///// EncodingCompare definitions /////

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