

using System;

namespace GameLogic
{
    public class Solver <StateType, MoveType> 
    where StateType : GameStateBase<MoveType, StateType> 
    {
        public Solver() {}

        /// <summary>
        /// Runs an Alpha Beta search through the entire game state tree. 
        /// </summary>
        /// <param name="state">The State to search.</param>
        /// <param name="alpha"></param>
        /// <param name="beta"></param>
        /// <returns>A tuple where Item1 is the best move in the position, and Item2 is the Value of that move.</returns>
        public (MoveType, int) search(StateType state, int alpha = int.MinValue, int beta = int.MaxValue) 
        {
            // Check for base case.
            if (state.isTerminal())
            {
                return (default(MoveType), state.utility());
            }
            
            // setup branching
            // Best Move and Utility pairs
            (MoveType, int) bestMoveValues = (default(MoveType), state.isMaxNode() ? int.MinValue : int.MaxValue);
            (MoveType, int) nextStateValues;
            MoveType[] actions = state.generateMoves();

            // branch for each possible action and evaluate them recursivly.
            foreach (MoveType action in actions)
            {
                nextStateValues = search(state.generateSuccessor(action), alpha, beta);
                if ((state.isMaxNode() ==  nextStateValues.Item2 > bestMoveValues.Item2)) // Compare the utility of the best move and the next state.
                {
                    bestMoveValues.Item1 = action;
                    bestMoveValues.Item2 = nextStateValues.Item2;
                }

                // Update alpha or beta.
                if (state.isMaxNode())
                {
                    alpha = Math.Max(alpha, bestMoveValues.Item2);
                    if (bestMoveValues.Item2 >= beta) { break; }
                }
                else
                {
                    beta = Math.Min(beta, bestMoveValues.Item2);
                    if (bestMoveValues.Item2 <= alpha) { break; }
                }
            }

            // Now bestMoveValues.Item2 is the true value of the state, and bestMoveValues.Item1 is the move associated with it.
            return bestMoveValues;
        }
    }
}