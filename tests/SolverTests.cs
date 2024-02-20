using NUnit.Framework;

using GameLogic;
using NuGet.Frameworks;


namespace SolverTests
{
    /// <summary>
    /// An implementation of GameStateBase for testing the search function in a more controlled enviornment
    /// </summary>
    public class TestGameState : GameStateBase<int, TestGameState>
    {
        /// <summary>
        /// Metric for how many nodes the search visited, needed to check that pruning is done properly.
        /// </summary>
        public static int nodesExplored { get; set; } = 0;

        /// <summary>
        /// Is this State a max node
        /// </summary>
        public bool maxNode;

        /// <summary>
        /// The number associated with this state.
        /// </summary>
        public int stateNumber { get; set; }

        /// <summary>
        /// Contains a state connection table. the first index is the array of states that the index state can reach.
        /// </summary>
        public int[][] reachableStates { get; set; }

        /// <summary>
        /// Contains the utility of each state.
        /// </summary>
        public int[] utilities { get; set; }

        public TestGameState() 
        {
            nodesExplored++;
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

    [TestFixture]
    public class SolverUnitTests
    {
        /// <summary>
        /// Tree:
        /// 0
        /// Utilities:
        /// 1
        /// </summary>
        [Test]
        public void terminalNode_returns_utility()
        {
            TestGameState.nodesExplored = 0; // Reset for proper count.
            TestGameState test = new TestGameState();
            test.reachableStates = new int[1][];
            test.reachableStates[0] = new int[0];
            test.utilities = new int[1] {1};
            Solver<TestGameState, int> solver = new Solver<TestGameState, int>();

            (int, int) searchResult = solver.search(test);

            Assert.That(searchResult.Item1, Is.EqualTo(default(int)));
            Assert.That(searchResult.Item2, Is.EqualTo(1));
            Assert.That(TestGameState.nodesExplored, Is.EqualTo(1));
        }

        [Test]
        public void smallTree_returns_move1Utility2()
        {
            TestGameState.nodesExplored = 0;
            TestGameState testState = new TestGameState();
            testState.reachableStates = new int[3][];
            testState.reachableStates[0] = new int[2] {1, 2};
            testState.reachableStates[1] = new int[0];
            testState.reachableStates[2] = new int[0];
            testState.utilities = new int[3] {0, 2, 0};
            Solver<TestGameState, int> solver = new Solver<TestGameState, int>();

            (int, int) searchResult = solver.search(testState);

            Assert.That(searchResult.Item1, Is.EqualTo(1)); // check right action
            Assert.That(searchResult.Item2, Is.EqualTo(2)); // check right utility
            Assert.That(TestGameState.nodesExplored, Is.EqualTo(3));

        }
    }
}