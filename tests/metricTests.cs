using NUnit.Framework;

using GameLogic;
using System;

namespace MetricTests
{
    [TestFixture]
    public class MetricTests
    {
        [Test]
        public void StartUltimateTicTacToeState_SolverTimed()
        {
            // Setup the search
            UltimateTicTacToeState state = new UltimateTicTacToeState();
            Solver<UltimateTicTacToeState, TicTacToeMove> solver = new Solver<UltimateTicTacToeState, TicTacToeMove>();

            // Time the search
            var timer = System.Diagnostics.Stopwatch.StartNew();
            (TicTacToeMove, int) result = solver.search(state);
            timer.Stop();

            // Output search time
            Console.WriteLine("Time to search staring state: " + timer.ElapsedMilliseconds.ToString() + " Milliseconds");
            Console.WriteLine(result.Item2.ToString());
        }
    }
}