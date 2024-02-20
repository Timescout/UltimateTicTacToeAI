using System.Collections.Generic;
using NUnit.Framework;

using GameLogic;


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
        public List<List<int>> reachableStates { get; set; }

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
            successor.utilities = utilities;
            successor.stateNumber = action;
            successor.maxNode = !maxNode;
            return successor;
        }

        public override List<int> generateMoves()
        {
            return reachableStates[stateNumber];
        }

        public override bool isTerminal()
        {
            return reachableStates[stateNumber].Count == 0; 
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
            test.reachableStates = new List<List<int>> { new List<int>()};
            test.utilities = new int[1] {1};
            Solver<TestGameState, int> solver = new Solver<TestGameState, int>();

            (int, int) searchResult = solver.search(test);

            Assert.That(searchResult.Item1, Is.EqualTo(default(int)));
            Assert.That(searchResult.Item2, Is.EqualTo(1));
            Assert.That(TestGameState.nodesExplored, Is.EqualTo(1));
        }


        /// <summary>
        /// Tree:
        /// 0
        /// -1
        /// -2
        /// Utilities:
        /// 0, 2, 0
        /// </summary>
        [Test]
        public void smallTree_returns_move1Utility2()
        {
            TestGameState.nodesExplored = 0;
            TestGameState testState = new TestGameState();
            testState.reachableStates = new List<List<int>>
            {
                new List<int> {1, 2},
                new List<int> {},
                new List<int> {}
            };
            testState.utilities = new int[3] {0, 2, 0};
            Solver<TestGameState, int> solver = new Solver<TestGameState, int>();

            (int, int) searchResult = solver.search(testState);

            Assert.That(searchResult.Item1, Is.EqualTo(1), "Action number was expected to be 1");
            Assert.That(searchResult.Item2, Is.EqualTo(2), "Utility was expected to be 2");
            Assert.That(TestGameState.nodesExplored, Is.EqualTo(3), "Expected nodesExplored to be 3");

        }


        /// <summary>
        /// Tree:
        /// 0
        /// -1
        /// -2
        /// --3
        /// --4 // should be pruned
        /// utilities:
        /// 0, 1, 0, 0, 0
        /// </summary>
        [Test]
        public void middleTree_minNodePrunes_1node()
        {
            TestGameState.nodesExplored = 0;
            TestGameState testState = new TestGameState();
            testState.reachableStates = new List<List<int>>
            {
                new List<int> {1, 2},
                new List<int> {},
                new List<int> {3, 4},
                new List<int> {},
                new List<int> {}
            };
            testState.utilities = new int[5] {0, 1, 0, 0, 0};
            Solver<TestGameState, int> solver = new Solver<TestGameState, int>();

            (int, int) searchResult = solver.search(testState);

            Assert.That(searchResult.Item1, Is.EqualTo(1), "Expected action number to be 1");
            Assert.That(searchResult.Item2, Is.EqualTo(1), "Expected utility to be 1");
            Assert.That(TestGameState.nodesExplored, Is.EqualTo(4), "Expected nodesExplored to be 4");
        }

        /// <summary>
        /// Tree:
        /// -0
        /// --1
        /// --2
        /// ---3
        /// ---4
        /// ----5
        /// ----6
        /// Utilities:
        /// 0, 1, 0, 2, 0, 3, 0
        /// </summary>
        [Test]
        public void largeTree_maxNodePrunes_1node()
        {
            TestGameState.nodesExplored = 0;
            TestGameState testState = new TestGameState();
            testState.reachableStates = new List<List<int>>
            {
                new List<int> {1, 2},
                new List<int> {},
                new List<int> {3, 4},
                new List<int> {},
                new List<int> {5, 6},
                new List<int> {},
                new List<int> {}
            };
            testState.utilities = new int[7] {0, 1, 0, 2, 0, 3, 0};
            Solver<TestGameState, int> solver = new Solver<TestGameState, int>();

            (int, int) searchResult = solver.search(testState);

            Assert.That(searchResult.Item1, Is.EqualTo(2), "Expected action to be 2");
            Assert.That(searchResult.Item1, Is.EqualTo(2), "Expected utility to be 2");
            Assert.That(TestGameState.nodesExplored, Is.EqualTo(6), "Expected nodesExplored to be 6");
        }
    }
}