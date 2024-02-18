using System;
using System.Numerics;
using GameLogic;



namespace Tests
{
    public class TestGameState : GameStateBase<int>
    {
        private int stateNumber { get; set; }
        private int[][] reachableStates { get; set; }
        private int[] utilities { get; set; }

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

        public override int[] generateMoves()
        {
            return reachableStates[stateNumber];
        }

        public override bool isTerminal()
        {
            return reachableStates[stateNumber].Length == 0; 
        }
    }
}