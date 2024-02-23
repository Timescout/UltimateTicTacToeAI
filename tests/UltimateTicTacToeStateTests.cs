using NUnit.Framework;
using System;
using System.Collections.Generic;

using GameLogic;

namespace UltimateTicTacToeStateTests
{
    [TestFixture]
    public class UltimateTicTacToeStateTester
    {
        [Test]
        public void ticTacToeMove00_creates_validMove()
        {
            Assert.DoesNotThrow(() => {TicTacToeMove move = new TicTacToeMove(0, 0);}, "0, 0 is a valid move but throws an error");
        }

        [Test]
        public void ticTacToeMove88_creates_validMove()
        {
            Assert.DoesNotThrow(() => {TicTacToeMove move = new TicTacToeMove(8, 8);}, "8, 8 is a valid move but throws an error");
        }

        [Test]
        public void ticTacToeMove09_throws_outOfRangeError()
        {
            Assert.Throws(typeof(ArgumentOutOfRangeException), () => {TicTacToeMove move = new TicTacToeMove(0, 9);}, "0, 9 is an invalid move but does not throw an exception");
        }

        [Test]
        public void ticTacToeMove90_throws_outOfRangeError()
        {
            Assert.Throws(typeof(ArgumentOutOfRangeException), () => {TicTacToeMove move = new TicTacToeMove(9, 0);}, "9, 0 is an invalid move but does not throw an exception");
        }

        [Test]
        public void ticTacToeMove0n1_throws_outOfRangeError()
        {
            Assert.Throws(typeof(ArgumentOutOfRangeException), () => {TicTacToeMove move = new TicTacToeMove(0, -1);}, "0, -1 is an invalid move but does not throw an exception");
        }

        [Test]
        public void ticTacToeMoven10_throws_outOfRangeError()
        {
            Assert.Throws(typeof(ArgumentOutOfRangeException), () => {TicTacToeMove move = new TicTacToeMove(-1, 0);}, "-1, 0 is an invalid move but does not throw an exception");
        }

        [Test]
        public void evaluationX0_returns_100()
        {
            Assert.That(UltimateTicTacToeState.evaluation('X', 0), Is.EqualTo(100), "Evaluation with X winning at a depth of 0 should return 100");
        }

        [Test]
        public void evaluationO0_returns_n100()
        {
            Assert.That(UltimateTicTacToeState.evaluation('O', 0), Is.EqualTo(-100), "Evaluation with O winning at a depth of 0 should return -100");
        }

        [Test]
        public void evaluationD_returns_0()
        {
            Assert.That(UltimateTicTacToeState.evaluation('D', 0), Is.EqualTo(0), "Evaluation of a draw should return 0");
        }

        [Test]
        public void evaluationX1_returns_99()
        {
            Assert.That(UltimateTicTacToeState.evaluation('X', 1), Is.EqualTo(99), "Evaluation with X winning at a depth of 1 should return 99");
        }

        [Test]
        public void evaluationO1_returns_n99()
        {
            Assert.That(UltimateTicTacToeState.evaluation('O', 1), Is.EqualTo(-99), "Evaluation with O winning at a depth of 1 should return -99");
        }

        [Test]
        public void isTerminal_StartingState_returnsFalse()
        {
            UltimateTicTacToeState state = new UltimateTicTacToeState();

            Assert.That(state.isTerminal(), Is.False, "Starting State should not be terminal");
        }

        [Test]
        public void TicTacToeEvaluation_ListOfNs_returnsN()
        {
            List<char> list = new List<char>();
            for (int i = 0; i < 9; i++)
            {
                list.Add('N');
            }

            char result = UltimateTicTacToeState.TicTacToeEvaluation(list);

            Assert.That(result, Is.EqualTo('N'), "TicTacToeEvaluation should return N on list of N's");
        }

        [Test]
        public void GenerateMoves_StartState_generates81Moves()
        {
            UltimateTicTacToeState state = new UltimateTicTacToeState();
            List<TicTacToeMove> moves;

            moves = state.generateMoves();

            Assert.That(moves.Count, Is.EqualTo(81), "Starting State should have 81 legal moves");
        }

    }
}