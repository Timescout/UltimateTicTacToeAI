

using System;

namespace GameLogic
{
    public class Solver <MoveType>
    {
        public Solver() {}
        public int search(GameStateBase<MoveType> state, int alpha = int.MinValue, int beta = int.MaxValue) 
        {
            // Check for base case.
            if (state.isTerminal())
            {
                return state.utility();
            }
            
            // setup branching
            int utility = state.isMaxNode() ?  int.MinValue : int.MaxValue;
            int nextStateUtility;
            MoveType[] actions = state.generateMoves();
            MoveType bestMove;

            // branch for each possible action and evaluate them recursivly.
            foreach (MoveType action in actions)
            {
                nextStateUtility = search(state.generateSuccessor(action), alpha, beta);
                if ((state.isMaxNode() == utility > nextStateUtility))
                {
                    bestMove = action;
                    utility = nextStateUtility;
                }

                // Update alpha or beta.
                if (state.isMaxNode())
                {
                    alpha = Math.Max(alpha, utility);
                    if (utility >= beta) { break; }
                }
                else
                {
                    beta = Math.Min(beta, utility);
                    if (utility <= alpha) { break; }
                }
            }

            // Now utility is the true value of the state. 
            return utility;
        }
    }
}