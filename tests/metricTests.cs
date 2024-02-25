using NUnit.Framework;

using GameLogic;
using System;

namespace MetricTests
{
    public class UltimateTicTacToeStateMetricStates : UltimateTicTacToeState
    {
        public static UltimateTicTacToeState createNormalTicTacToeGame()
        {
            UltimateTicTacToeState state = new UltimateTicTacToeState();
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    state.board[i][j] = 'D';
                }
                state.boardStatus[i] = UltimateTicTacToeState.TicTacToeEvaluation(state.board[i]);
            }
            return state;
        }
    }

    [TestFixture]
    public class MetricTests
    {
        //[Test] // Test ran for too long, and was stopped manually after 24 hours.
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

        [Test]
        public void smallTicTacToeState_SolverTimed()
        {
            UltimateTicTacToeState state = UltimateTicTacToeStateMetricStates.createNormalTicTacToeGame();
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