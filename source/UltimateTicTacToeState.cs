using System;
using System.Collections.Generic;
using System.Xml;
using NUnit.Framework;


namespace GameLogic
{
    public struct TicTacToeMove
    {
        public int board {get;}

        public int space {get;}

        public TicTacToeMove(int boardNum, int spaceNum)
        {
            // Check that the move is in bounds
            if (boardNum < 0 || boardNum > 8)
            {
                throw new ArgumentOutOfRangeException("TicTacToeMove boardNum must be between 0 and 8");
            }
            if (spaceNum < 0 || spaceNum > 8)
            {
                throw new ArgumentOutOfRangeException("TicTacToeMove spaceNum must be between 0 and 8");
            }

            board = boardNum;
            space = spaceNum;
        }
    }

    public class UltimateTicTacToeState : GameStateBase<TicTacToeMove, UltimateTicTacToeState>
    {
        // Counts the total number of moves by the players. This is used in utility() so that moves which end the game quicker are prioritized.
        public int depth {get; private set;} 

        // use isMaxNode() for access.
        private bool isXToPlay = true;

        // used to track the status of the small nine boards. Because who has won a board can change if new moves are played there, and the rules allow for moves to continue to be played in won boards, this is needed to prevent boards changing hands.
        public List<char> boardStatus {get; protected set;}

        public List<List<char>> board {get; protected set;}

        public enum boardNumber 
        {
            board0 = 0,
            board1 = 1,
            board2 = 2,
            board3 = 3,
            board4 = 4,
            board5 = 5,
            board6 = 6,
            board7 = 7,
            board8 = 8,
            anyBoard
        }
        public boardNumber activeBoard {get; protected set;}

        private void init()
        {
            activeBoard = boardNumber.anyBoard;
            depth = 0;
            isXToPlay = true;
            // set up the board
            board = new List<List<char>>();
            for (int i = 0; i < 9; i++)
            {
                board.Add(new List<char>());
                for (int j = 0; j < 9; j++)
                {
                    board[i].Add('N'); // N for none
                }
            }

            // set up the board status
            boardStatus = new List<char>();
            for (int i = 0; i < 9; i++)
            {
                boardStatus.Add('N'); // N for none
            }
        }

        public UltimateTicTacToeState()
        {
            init();
        }

        public UltimateTicTacToeState(UltimateTicTacToeState copyState) // I think this is creating a copy, then copying the copy. Feels bad but it yells at me when I try to pass by reference.
        {
            init();
            depth = copyState.depth;
            activeBoard = copyState.activeBoard;
            isXToPlay = copyState.isXToPlay;
            for (int i = 0; i < copyState.boardStatus.Count; i++)
            {
                boardStatus[i] = copyState.boardStatus[i];
            }
            for (int i = 0; i < copyState.board.Count; i++)
            {
                for (int j = 0; j < copyState.board[i].Count; j++)
                {
                    board[i][j] = copyState.board[i][j];
                }
            }
        }


        public override List<TicTacToeMove> generateMoves()
        {
            List<TicTacToeMove> moves = new List<TicTacToeMove>();
            for (int board = 0; board < 9; board++)
            {   
                if (activeBoard == boardNumber.anyBoard || activeBoard == (boardNumber)board)
                {
                    for (int space = 0; space < 9; space++)
                    {
                        if (this.board[board][space] == 'N')
                        {
                            moves.Add(new TicTacToeMove(board, space));
                        }
                    }
                }
            }
            return moves;
        }

        public override bool isMaxNode()
        {
            return isXToPlay;
        }

        public override bool isTerminal()
        {
            return TicTacToeEvaluation(boardStatus) != 'N'; // if noone has won, the game is not terminal.
        }

        public override int utility()
        {
            if (depth == 1)
            {
                Console.WriteLine("we have a problem.");
            }
            return evaluation(TicTacToeEvaluation(boardStatus), depth);
        }

        public static int evaluation(char result, int depth)
        {
            int utility = 0;
            switch (result) // based on the status of the board, return a utility.
            {
                case 'X':
                    utility = 100;
                    utility -= depth;
                    break;
                case 'O':
                    utility = -100;
                    utility += depth;
                    break;
                default:
                    break;
            }
            return utility;
        }

        public override UltimateTicTacToeState generateSuccessor(TicTacToeMove action)
        {
            UltimateTicTacToeState successor = new UltimateTicTacToeState(this);
            successor.depth++;
            successor.isXToPlay = !isXToPlay; // if X is to play, now O is to play and vice versa.
            successor.board[action.board][action.space] = isXToPlay ? 'X' : 'O'; // play the move
            if (boardStatus[action.board] == 'N') // if the board has not been won, update the board status.
            {
                boardStatus[action.board] = TicTacToeEvaluation(board[action.board]);
            }
            successor.activeBoard = boardNumber.anyBoard;
            for (int i = 0; i < 9; i++) // check if there is space in the board that is supposed to be the next active board for moves to be played.
            {
                if (successor.board[action.space][i] == 'N')
                {
                    successor.activeBoard = (boardNumber)action.space;
                }
            }
            return successor;
        }

        public static char TicTacToeEvaluation(List<char> board)
        {
            char[] players = {'X', 'O'};
            // check for winning players.
            foreach (char player in players)
            {
                if 
                (
                    // horixontal tripilets
                    (board[0] == player && board[1] == player && board[2] == player) ||
                    (board[3] == player && board[4] == player && board[5] == player) ||
                    (board[6] == player && board[7] == player && board[8] == player) ||
                    // vertical triplets
                    (board[0] == player && board[3] == player && board[6] == player) ||
                    (board[1] == player && board[4] == player && board[7] == player) ||
                    (board[2] == player && board[5] == player && board[8] == player) ||
                    // both diagonals
                    (board[0] == player && board[4] == player && board[8] == player) ||
                    (board[2] == player && board[4] == player && board[6] == player)
                )
                {
                    return player;
                }
            }

            foreach (char space in board)
            {
                if (space == 'N')
                {
                    return 'N'; // if there are empty spots, noone has won the board yet.
                }
            }

            return 'D'; // D for draw. If all spots are full and noone has won, then the board is a draw. They are artists because they drew the game.
        }
    }
}