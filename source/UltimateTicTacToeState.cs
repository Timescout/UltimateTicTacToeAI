using System;
using System.Collections.Generic;


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
        private int depth_ = 0; // Counts the total number of moves by the players. This is used in utility() so that moves which end the game quicker are prioritized.

        private bool isXToPlay = true;

        // used to track the status of the small nine boards. Because who has won a board can change if new moves are played there, and the rules allow for moves to continue to be played in won boards, this is needed to prevent boards changing hands.
        private List<char> boardStatus_;

        private List<List<char>> board_ {get; set;}

        private void init()
        {
            // set up the board
            board_ = new List<List<char>>();
            for (int i = 0; i < 9; i++)
            {
                board_.Add(new List<char>());
                for (int j = 0; j < 9; j++)
                {
                    board_[i].Add('E'); // E for empty
                }
            }

            // set up the board status
            boardStatus_ = new List<char>();
            for (int i = 0; i < 9; i++)
            {
                boardStatus_.Add('N'); // N for none
            }
        }

        public UltimateTicTacToeState()
        {
            init();
        }

        public UltimateTicTacToeState(UltimateTicTacToeState copyState) // I think this is creating a copy, then copying the copy. Feels bad but it yells at me when I try to pass by reference.
        {
            board_ = copyState.board_;
            activeBoard_ = copyState.activeBoard_;
            depth_ = copyState.depth_;
            boardStatus_ = copyState.boardStatus_;
        }

        private enum boardNumber 
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
        private boardNumber activeBoard_ = boardNumber.anyBoard;

        public override List<TicTacToeMove> generateMoves()
        {
            List<TicTacToeMove> moves = new List<TicTacToeMove>();
            for (int board = 0; board < 9; board++)
            {   
                if (activeBoard_ == boardNumber.anyBoard || activeBoard_ == (boardNumber)board)
                {
                    for (int space = 0; space < 9; space++)
                    {
                        if (board_[board][space] == default(char))
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
            return TicTacToeEvaluation(boardStatus_) == 'N'; // if noone has won, the game is not terminal.
        }

        public override int utility()
        {
            int utility = 0;
            switch (TicTacToeEvaluation(boardStatus_)) // based on the status of the board, return a utility.
            {
                case 'X':
                    utility = 100;
                    utility -= depth_;
                    break;
                case 'O':
                    utility = -100;
                    utility += depth_;
                    break;
                default:
                    break;
            }
            return utility;
        }

        public override UltimateTicTacToeState generateSuccessor(TicTacToeMove action)
        {
            UltimateTicTacToeState successor = new UltimateTicTacToeState(this);
            successor.depth_++;
            successor.isXToPlay = !isXToPlay; // if X is to play, now O is to play and vice versa.
            successor.board_[action.board][action.space] = isXToPlay ? 'X' : 'O'; // play the move
            if (boardStatus_[action.board] == 'N') // if the board has not been won, update the board status.
            {
                boardStatus_[action.board] = TicTacToeEvaluation(board_[action.board]);
            }
            successor.activeBoard_ = boardNumber.anyBoard;
            for (int i = 0; i < 9; i++) // check if there is space in the board that is supposed to be the next active board for moves to be played.
            {
                if (successor.board_[action.space][i] == default(char))
                {
                    successor.activeBoard_ = (boardNumber)action.space;
                }
            }
            return successor;
        }

        private char TicTacToeEvaluation(List<char> board)
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