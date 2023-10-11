
The goal of this project is to create an AI that can play Ultimate Tic Tac Toe (U3T). This is a solved game so the outcome is known if both players play optimaly. That combined with the small state space of U3T determined the strategy that this AI will use to play. The AI will create a table of states and moves where the AI can look up the correct move to play in each state. To create this table, it will use a minimax algorithm to search through the state space. 


##### State encoding
Each game state is encoded into binary and saved in a table. This allows for any state to be looked up to quickly find a good move in the position. The information that must be encoded is this:

1. who is to play
2. which of the nine boards is active
3. the evaluation of the state
4. the best move
5. the spaces that have already been played in (where the X's and O's are)
6. Which boards have been won and by whome. 

Who is to play requires only 1 bit, and will be encoded as 
| Value |   State   |
|-------| ----------|
| 0     | O to play |
| 1     | X to play |

which of the nine boards is active has 10 posilities including any board, and so 4 bits must be used. 

| Value |  State  |
|-------| --------|
| 0000  | Board 0 |
| 0001  | Board 1 |
| 0010  | Board 2 |
| 0011  | Board 3 |
| 0100  | Board 4 |
| 0101  | Board 5 |
| 0110  | Board 6 |
| 0111  | Board 7 |
| 1000  | Board 8 |
| 1111  | Any Board may be played on |

The evaluation of the state has three possibilities. They are arrainged so that O winning has the smallest value, and X winning has the highest value. This makes comparing evaluations easier.

| Value |  State  |
|-------| --------|
| 00    | O wins  |
| 01    | Draw    | 
| 10    | X wins  |

The best move has two parts, which active board is to be played on and which space on that board to play on. Since each of those requires 4 bits the move is encoded in 8 bits with two four bit parts. There may be some clever way to store the board to play on in the active board field but that can be left for the future.

| Value |  Best move space  |
|-------| --------|
| 0000  | 0  | 
| 0001  | 1  |
| 0010  | 2  |
| 0011  | 3  | 
| 0100  | 4  |
| 0101  | 5  |
| 0110  | 6  | 
| 0111  | 7  |
| 1000  | 8  |

| Value |  Board  |
|-------| --------|
| 0000  | 0  | 
| 0001  | 1  |
| 0010  | 2  |
| 0011  | 3  | 
| 0100  | 4  |
| 0101  | 5  |
| 0110  | 6  | 
| 0111  | 7  |
| 1000  | 8  |

With encoding the location of the X's and O's there are three states a space can be in. It can be empty, an X, and an O. Thus we need two bits to encode all of the information. This encoding will be used for each board, as well as for who has won each board. Because this is also used for results, draw is included.

| Value |  State  |
|-------| --------|
| 00    | Space is empty    | 
| 11    | X Played in the space  |
| 10    | O Played in the space  |
| 01    | Draw |

The boards will be encoded starting in the top left space and continuing to the right, from top to bottom. The least significant bit of the encoding will be the lower right space. The whole board will use the same scheme, where the top left board is encoded which continues right top to bottom.