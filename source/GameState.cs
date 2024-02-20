
using System.Collections.Generic;

namespace GameLogic
{
    /// <summary>
    /// Abstract class for a two player game with perfect information.
    /// </summary>
    /// <typeparam name="MoveType">The move class associated with this game.</typeparam>
    /// <typeparam name="GameState">Because we want generateSuccessor to return an instance of the class which inherits from GameStateBase, GameStateBase must take that type as an argument. Feels very tricky and confusing.</typeparam>
    public abstract class GameStateBase <MoveType, GameState>
    {
        /// <summary>
        /// Must Return an array of legal moves in this state. If this list is empty then the state must be Terminal and isTerminal() must return true.
        /// </summary>
        public abstract List<MoveType> generateMoves();

        /// <summary>
        /// Returns whether the current state is terminal. If there are no legal moves than the state must be terminal.
        /// </summary>
        public abstract bool isTerminal();

        /// <summary>
        /// Must return a state where the given action was takken in this state. The return type is the child class of this base class. To get an array of possible actions, use generateMoves(). If this list is empty, then the state is terminal and isTerminal will be true.
        /// </summary>
        /// <param name="action">The legal action takken in this state.</param>
        public abstract GameState generateSuccessor(MoveType action);

        /// <summary>
        /// Must Return the utility of this state where high values are good for max nodes and low values are good for min nodes.
        /// </summary>
        public abstract int utility();

        /// <summary>
        /// Must return whether this state is a max node or not.
        /// </summary>
        public abstract bool isMaxNode();
    }
}