

using System.Numerics;

namespace GameLogic
{
    /// <summary>
    /// Abstract class for a two player game with perfect information.
    /// </summary>
    /// <typeparam name="MoveType">The move class associated with this game.</typeparam>
    public abstract class GameStateBase <MoveType>
    {

        public abstract MoveType[] generateMoves();

        public abstract bool isTerminal();

        public abstract GameStateBase<MoveType> generateSuccessor(MoveType action);

        public abstract int utility();
    }
}