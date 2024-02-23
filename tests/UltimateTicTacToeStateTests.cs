using NUnit.Framework;

using GameLogic;
using System;
using System.Security.Cryptography.X509Certificates;

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
    }
}