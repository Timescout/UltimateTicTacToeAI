using System.Numerics;
using GameLogic;



namespace Tests
{
    public class TestGameState : GameStateBase<int>
    {
        private int stateNumber { get; set; }
        private Vector<Vector<int>> reachableStates { get; set; }
        private Vector<int> utilities { get; set; }

        public TestGameState() {}

        public override int utility()
        {
            return utilities[stateNumber];
        }

        public override TestGameState generateSuccessor(int action)
        {
            TestGameState successor = new TestGameState();
            successor.reachableStates = reachableStates;
            successor.stateNumber = action;
            return successor;
        }

        public override Vector<int> generateMoves()
        {
            return reachableStates[stateNumber];
        }

        public override bool isTerminal()
        {
            return false; // TODO somehow figure out how to compare the length of reachableStates[stateNumber] to 0.
        }
    }
}