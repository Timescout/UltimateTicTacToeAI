using GameLogic;



namespace GameLogicTests
{
    public class TestGameState : GameStateBase<int>
    {
        public static int nodesExplored = 0;

        private bool maxNode;
        private int stateNumber { get; set; }
        private int[][] reachableStates { get; set; }
        private int[] utilities { get; set; }

        public TestGameState() 
        {
            maxNode = true;
        }

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

        public override int[] generateMoves()
        {
            return reachableStates[stateNumber];
        }

        public override bool isTerminal()
        {
            return reachableStates[stateNumber].Length == 0; 
        }
        public override bool isMaxNode()
        {
            return maxNode;
        }
    }
}