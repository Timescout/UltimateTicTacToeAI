using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;


namespace GameLogic
{
    public struct TicTacToeMove
    {
        [Range(0, 8, ErrorMessage = "TicTacToeMove board must be between 0 and 8 inclusive")]
        public int board {get; set;}
        [Range(0, 8, ErrorMessage = "TicTacToeMove space must be between 0 and 8 inclusive")]
        public int space {get; set;}

        public TicTacToeMove(int boardNum, int spaceNum)
        {
            board = boardNum;
            space = spaceNum;
        }
    }

    public class UltimateTicTacToeState : GameStateBase<TicTacToeMove, UltimateTicTacToeState>
    {
        private int depth_ = 0; // Counts the total number of moves by the players. This is used in utility() so that moves which end the game quicker are prioritized.

        private bool isXToPlay = true;

        private List<List<char>> board_ {get; set;}

        private void init()
        {
            board_ = new List<List<char>>();
            for (int i = 0; i < 9; i++)
            {
                board_.Add(new List<char>());
                for (int j = 0; j < 9; j++)
                {
                    board_[i].Add('E'); // E for empty
                }
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
            throw new System.NotImplementedException();
        }

        public override int utility()
        {
            int utility = 0;
            List<char> smallBoardReaults = new List<char>();
            for (int i = 0; i < 9; i++)
            {
                smallBoardReaults[i] = TicTacToeEvaluation(board_[i]);
            }
            char result = TicTacToeEvaluation(smallBoardReaults); // tells if anyone has won the game.
            switch (result)
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
            successor.activeBoard_ = boardNumber.anyBoard;
            for (int i = 0; i < 9; i++) // check if there is 
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
            
            return 'D'; // D for draw. They are artists.
        }
    }
}