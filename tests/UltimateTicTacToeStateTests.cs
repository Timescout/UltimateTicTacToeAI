using NUnit.Framework;
using System;
using System.Collections.Generic;

using GameLogic;

namespace UltimateTicTacToeStateTests
{
    // inherits from UltimateTicTacToeState so it can access protected variables.
    public class UltimateTicTacToeStateFactory : UltimateTicTacToeState
    {
        // Creates a state where the top left bottom right diagonal and all horizontal win conditions must be right to be terminal
        public static UltimateTicTacToeState createTerminalStateXWins()
        {
            UltimateTicTacToeState state = new UltimateTicTacToeState();
            state.board[0][0] = 'X';
            state.board[0][1] = 'X';
            state.board[0][2] = 'X';
            
            state.board[4][3] = 'X';
            state.board[4][4] = 'X';
            state.board[4][5] = 'X';

            state.board[8][6] = 'X';
            state.board[8][7] = 'X';
            state.board[8][8] = 'X';

            for (int i = 0; i < 9; i++)
            {
                state.boardStatus[i] = TicTacToeEvaluation(state.board[i]);
            }
            return state;
        }

        // Creates a state where the top right to bottom left diagonal and all vertical win conditiona must be right to be terminal
        public static UltimateTicTacToeState createTerminalStateOWins()
        {
            UltimateTicTacToeState state = new UltimateTicTacToeState();
            state.board[2][0] = 'O';
            state.board[2][3] = 'O';
            state.board[2][6] = 'O';
            
            state.board[4][1] = 'O';
            state.board[4][4] = 'O';
            state.board[4][7] = 'O';

            state.board[6][2] = 'O';
            state.board[6][5] = 'O';
            state.board[6][8] = 'O';
            for (int i = 0; i < 9; i++)
            {
                state.boardStatus[i] = TicTacToeEvaluation(state.board[i]);
            }
            return state;
        }
    }


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

        [Test]
        public void isTerminal_TerminalXWins_returnsTrue()
        {
            UltimateTicTacToeState state = UltimateTicTacToeStateFactory.createTerminalStateXWins();

            bool result = state.isTerminal();

            Assert.That(result, Is.True,"Terminal State should return true from isTerminal");
        }

        [Test]
        public void isTerminal_TerminalOWins_returnsTrue()
        {
            UltimateTicTacToeState state = UltimateTicTacToeStateFactory.createTerminalStateOWins();

            bool result = state.isTerminal();

            Assert.That(result, Is.True,"Terminal State should return true from isTerminal");
        }

        [Test]
        public void TicTacToeEvaluation_oneX_returnsN()
        {
            List<char> list = new List<char>();
            for (int i = 0; i < 9; i++)
            {
                list.Add('N');
            }
            list[0] = 'X';

            char result = UltimateTicTacToeState.TicTacToeEvaluation(list);

            Assert.That(result, Is.EqualTo('N'), "List With only one X should return N from TicTacToeEvaluation");
        }

        [Test]
        public void evaluation_DifferentDepths_prioritizesLowerDepth()
        {
            int highValue = UltimateTicTacToeState.evaluation('X', 1);
            int lowValue = UltimateTicTacToeState.evaluation('X', 2);

            bool result = highValue > lowValue;

            Assert.That(result, Is.True, "Wins with lower depth should be prioritized");
        }

    }
}