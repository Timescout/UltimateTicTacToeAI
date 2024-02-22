using NUnit.Framework;

using GameLogic;
using System;

namespace UltimateTicTacToeStateTests
{
    [TestFixture]
    public class UltimateTicTacToeStateTester
    {
        // [Test]
        // public void SetSpacePlayed_invalidSpace_throwsOutOfRangeError()
        // {
        //     UltimateTicTacToeState state = new UltimateTicTacToeState();

        //     Assert.Throws();
        // }

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
    }
}