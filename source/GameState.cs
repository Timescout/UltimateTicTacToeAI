

namespace GameLogic
{
    /// <summary>
    /// Abstract class for a two player game with perfect information.
    /// </summary>
    /// <typeparam name="MoveType">The move class associated with this game.</typeparam>
    public abstract class GameStateBase <MoveType>
    {

        public abstract MoveType[] generateMoves();

        /// <summary>
        /// Returns whether the current state is terminal. If there are no legal moves than the state must be terminal.
        /// </summary>
        public abstract bool isTerminal();

        public abstract GameStateBase<MoveType> generateSuccessor(MoveType action);

        public abstract int utility();

        public abstract bool isMaxNode();
    }
}