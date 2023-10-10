/* Andrew Bergman
9-14-23
Tests for the Ultimate3TState class.
*/

#include "State.h"
#include <gtest/gtest.h>

TEST(Ultimate3TStateTests, SetSpacePlayed_invalidSpace_ThrowsError)
{
    Ultimate3TState state;

    EXPECT_THROW(state.setSpacePlayed(0, 9, player::x), std::out_of_range);
}

TEST(Ultimate3TStateTests, SetSpacePlayed_HighBorderSpace_DoesNotThrowError)
{
    Ultimate3TState state;

    EXPECT_NO_THROW(state.setSpacePlayed(8, 8, player::o));
}

TEST(Ultimate3TStateTests, SetSpacePlayed_HighBorderSpaceActiveBoard_DoesNotThrowError)
{
    Ultimate3TState state;

    EXPECT_NO_THROW(state.setSpacePlayed(activeBoard::board8, 8, player::x));
}

TEST(Ultimate3TStateTests, GetSpacePlayed_HighBorderValues_ReturnValue)
{
    Ultimate3TState state;

    EXPECT_NO_THROW(player borderSpaceState = state.getSpacePlayed(8, 8));
}

TEST(Ultimate3TStateTests, GenerateMoves_AnyBoard_GeneratesAllMoves)
{
    Ultimate3TState state;

    std::vector<move> moves = state.generateMoves();

    EXPECT_EQ(moves.size(), 81);
    // Check that each move is unique. Since there are only 81 possible moves, if there are 81 moves and they are all unique then all moves have been generated.
    for (int i = 0; i < 81; i++)
    {
        for (int j = i+1; j < 81; j++)
        {
            EXPECT_NE(moves[i].toBinary(), moves[j].toBinary());
        }
    }
}

TEST(Ultimate3TStateTests, GenerateMoves_SpecificBoard_GeneratesAllMoves)
{
    Ultimate3TState state;
    state.setActiveBoard(activeBoard::board0);

    std::vector<move> moves = state.generateMoves();

    // Check that there are 9 moves, that each move is in the correct board, and that each move is unique.
    EXPECT_EQ(moves.size(), 9);
    for (int i = 0; i < 9; i++)
    {
        EXPECT_EQ(moves[i].board, activeBoard::board0);
        for (int j = i + 1; j < 9; j++)
        {
            EXPECT_NE(moves[i].toBinary(), moves[j].toBinary());
        }
    }
}

TEST(Ultimate3TStateTests, GenerateMoves_SpecificBoardWithPlayedSpace_GeneratesCorrectMoves)
{
    Ultimate3TState state;
    state.setActiveBoard(activeBoard::board0);
    state.setSpacePlayed(0, 0, player::x);

    std::vector<move> moves = state.generateMoves();

    // check that the right number of moves generated, and that they are not the one that has already been played.
    EXPECT_EQ(moves.size(), 8);
    for (int i = 0; i < moves.size(); i++)
    {
        EXPECT_NE(move(activeBoard::board0, 0).toBinary(), moves[i].toBinary());
    }
}

TEST(Ultimate3TStateTests, GenerateMoves_SpecificBoardWithNoFreeSpaces_GeneratesCorrectMoves)
{
    Ultimate3TState state;
    state.setActiveBoard(activeBoard::board0);
    state.setSpacePlayed(0, 0, player::x);
    state.setSpacePlayed(0, 1, player::x);
    state.setSpacePlayed(0, 2, player::x);
    state.setSpacePlayed(0, 3, player::x);
    state.setSpacePlayed(0, 4, player::x);
    state.setSpacePlayed(0, 5, player::x);
    state.setSpacePlayed(0, 6, player::x);
    state.setSpacePlayed(0, 7, player::x);
    state.setSpacePlayed(0, 8, player::x);

    std::vector<move> moves = state.generateMoves();

    EXPECT_EQ(moves.size(), 72); // 81 - 9 = 72, since one board is full, there are 9 less moves than the maximum
}

TEST(Ultimate3TStateTests, GenerateMoves_AnyBoardWithOneMovePlayed_GeneratesCorrectMoves)
{
    Ultimate3TState state;
    state.setActiveBoard(activeBoard::anyBoard);
    state.setSpacePlayed(0, 0, player::x);

    std::vector<move> moves = state.generateMoves();

    EXPECT_EQ(moves.size(), 80);
}

// We will create a parameterized test where a sequence of moves is played, and where the result is an X win.
class U3TUtilityWinConditionTests :
    public testing::TestWithParam<std::vector<move>>
{};

TEST_P(U3TUtilityWinConditionTests, SequenceOfMoves_Results_XWin)
{
    std::vector<move> moves = GetParam();
    Ultimate3TState state;

    for( int i = 0; i < moves.size(); i++)
    {
        state.setSpacePlayed(moves[i].board, moves[i].space, player::x);
    }

    EXPECT_EQ(state.utility(), player::x);
}

TEST_P(U3TUtilityWinConditionTests, SequenceOfMoves_Results_OWin)
{
    std::vector<move> moves = GetParam();
    Ultimate3TState state;

    for (int i = 0; i < moves.size(); i++)
    {
        state.setSpacePlayed(moves[i].board, moves[i].space, player::o);
    }

    EXPECT_EQ(state.utility(), player::o);
}

INSTANTIATE_TEST_SUITE_P(Ultimate3TStateTests, U3TUtilityWinConditionTests, testing::Values(
    std::vector<move>
    ({
        move(activeBoard::board0, 0),
        move(activeBoard::board0, 1),
        move(activeBoard::board0, 2),
        move(activeBoard::board4, 3),
        move(activeBoard::board4, 4),
        move(activeBoard::board4, 5),
        move(activeBoard::board8, 6),
        move(activeBoard::board8, 7),
        move(activeBoard::board8, 8)
    }),
    std::vector<move>
    ({
        move(activeBoard::board2, 0),
        move(activeBoard::board2, 3),
        move(activeBoard::board2, 6),
        move(activeBoard::board4, 1),
        move(activeBoard::board4, 4),
        move(activeBoard::board4, 7),
        move(activeBoard::board6, 2),
        move(activeBoard::board6, 5),
        move(activeBoard::board6, 8)
    })
));

TEST(Ultimate3TStateTests, Utility_DoesNotUpdate_OnFirstMove)
{
    Ultimate3TState state;

    state.setSpacePlayed(0, 0, player::x);
    state.setSpacePlayed(1, 0, player::x);
    state.setSpacePlayed(2, 0, player::x);

    EXPECT_EQ(state.utility(), player::neither);
}

TEST(Ultimate3TStateTests, Utility_DoesNotOverride_PreviousWins)
{
    Ultimate3TState state;
    // create a state where player o has won the top three subBoards.
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            state.setSpacePlayed(i, j, player::o);
        }
    }
    player winner = state.utility();

    // try to override the winner with new plays by x
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            state.setSpacePlayed(i, j, player::x);
        }
    }

    EXPECT_EQ(winner, state.utility());
}

TEST(Ultimate3TStateTests, Utility_DrawConditionsOne_ReturnsDraw)
{
    Ultimate3TState state;
    for(int i = 0; i < 9; i++)
    {
        // o x o
        // x x o
        // x o x
        state.setSpacePlayed(i, 0, player::o);
        state.setSpacePlayed(i, 1, player::x);
        state.setSpacePlayed(i, 2, player::o);
        state.setSpacePlayed(i, 3, player::x);
        state.setSpacePlayed(i, 4, player::x);
        state.setSpacePlayed(i, 5, player::o);
        state.setSpacePlayed(i, 6, player::x);
        state.setSpacePlayed(i, 7, player::o);
        state.setSpacePlayed(i, 8, player::x);
    }

    player utility = state.utility();

    EXPECT_EQ(utility, player::draw);
}

TEST(Ultimate3TStateTests, Utility_DrawConditionsTwo_ReturnsDraw)
{
    Ultimate3TState state;
    for(int i = 0; i < 9; i++)
    {
        // o x o
        // o x x
        // x o o
        state.setSpacePlayed(i, 0, player::o);
        state.setSpacePlayed(i, 1, player::x);
        state.setSpacePlayed(i, 2, player::o);
        state.setSpacePlayed(i, 3, player::o);
        state.setSpacePlayed(i, 4, player::x);
        state.setSpacePlayed(i, 5, player::x);
        state.setSpacePlayed(i, 6, player::x);
        state.setSpacePlayed(i, 7, player::o);
        state.setSpacePlayed(i, 8, player::o);
    }

    player utility = state.utility();

    EXPECT_EQ(utility, player::draw);
}

TEST(Ultimate3TStateTests, Utility_DrawConditionsThree_ReturnsDraw)
{
    Ultimate3TState state;
    for(int i = 0; i < 9; i++)
    {
        // x o o 
        // o x x 
        // o x o
        state.setSpacePlayed(i, 0, player::x);
        state.setSpacePlayed(i, 1, player::o);
        state.setSpacePlayed(i, 2, player::o);
        state.setSpacePlayed(i, 3, player::o);
        state.setSpacePlayed(i, 4, player::x);
        state.setSpacePlayed(i, 5, player::x);
        state.setSpacePlayed(i, 6, player::o);
        state.setSpacePlayed(i, 7, player::x);
        state.setSpacePlayed(i, 8, player::o);
    }

    player utility = state.utility();

    EXPECT_EQ(utility, player::draw);
}

TEST(Ultimate3TStateTests, SetBoard_BoardTooBig_ThrowsError)
{
    std::vector<std::vector<player>> tooBigBoard(10, std::vector<player>(9, player::neither));
    Ultimate3TState state;

    EXPECT_THROW(state.setBoard(tooBigBoard), std::invalid_argument);
}

TEST(Ultimate3TStateTests, SetBoard_SubBoardTooBig_ThrowsError)
{
    std::vector<std::vector<player>> tooBigBoard(9, std::vector<player>(10, player::neither));
    Ultimate3TState state;

    EXPECT_THROW(state.setBoard(tooBigBoard), std::invalid_argument);
}

TEST(Ultimate3TStateTests, GenerateSuccessor_LegalMoveXToPlayNoFullSubBoard_GeneratesSuccessorCorrectly)
{
    Ultimate3TState state;
    state.setActivePlayer(player::x);
    state.setSpacePlayed(activeBoard::board4, 4, player::neither);
    state.setSpacePlayed(activeBoard::board4, 5, player::neither); // need to make sure that subboard 4 is not full after this move.
    state.setActiveBoard(activeBoard::anyBoard);
    move action(activeBoard::board4, 4);

    Ultimate3TState successorState = state.generateSuccessorState(action);

    EXPECT_EQ(successorState.getActiveBoard(), activeBoard::board4);
    EXPECT_EQ(successorState.getActivePlayer(), player::o);
    EXPECT_EQ(successorState.getSpacePlayed(activeBoard::board4, 4), player::x);
}

TEST(Ultimate3TStateTests, GenerateSuccessor_LegalMoveXtoPlayFullSubBoard_GeneratesSuccessorCorrectly)
{
    Ultimate3TState state;
    state.setActivePlayer(player::x);
    move action(board8, 8);
    state.setActiveBoard(action.board);
    for(int i = 0; i < 8; i++)
    {
        state.setSpacePlayed(action.board, i, player::o);
    }

    Ultimate3TState successorState = state.generateSuccessorState(action);

    EXPECT_EQ(successorState.getActiveBoard(), anyBoard);
    EXPECT_EQ(successorState.getActivePlayer(), player::o);
    EXPECT_EQ(successorState.getSpacePlayed(action.board, action.space), player::x);
}

TEST(Ultimate3TStateTests, GenerateSuccessor_LegalMoveOToPlayNotFullSubBoard_GeneratesSuccessorCorrectly)
{
    Ultimate3TState state;
    state.setActivePlayer(player::o);
    move action(board0, 0);
    state.setActiveBoard(action.board);

    Ultimate3TState successorState = state.generateSuccessorState(action);

    EXPECT_EQ(successorState.getActiveBoard(), board0);
    EXPECT_EQ(successorState.getActivePlayer(), player::x);
    EXPECT_EQ(successorState.getSpacePlayed(action.board, action.space), player::o);
}

TEST(Ultimate3TStateTests, GenerateSuccessor_IllegalMoveNonplayableBoardAndFullSpot_ThrowsError)
{
    Ultimate3TState state;
    state.setActiveBoard(board1);
    move action(board0, 8);
    state.setSpacePlayed(action.board, action.space, player::o);

    EXPECT_THROW(state.generateSuccessorState(action), std::invalid_argument);
}

TEST(Ultimate3TStateTests, GenerateSuccessor_IllegalMoveNonplayableBoard_ThrowsError)
{
    Ultimate3TState state;
    state.setActiveBoard(board1);
    move action(board0, 8);

    EXPECT_THROW(state.generateSuccessorState(action), std::invalid_argument);
}

TEST(Ultimate3TStateTests, GenerateSuccessor_IllegalMoveFullSpot_ThrowsError)
{
    Ultimate3TState state;
    move action(board0, 8);
    state.setActiveBoard(anyBoard);
    state.setSpacePlayed(action.board, action.space, player::o);

    EXPECT_THROW(state.generateSuccessorState(action), std::invalid_argument);
}

TEST(Ultimate3TStateTests, IsTerminal_XWins_ReturnsTrue)
{
    Ultimate3TState state;

    for (int i = 0; i < 9; i++)
    {
        state.setSpacePlayed(i/3, i % 3, player::x);
    }

    EXPECT_TRUE(state.isTerminalState());
}

TEST(Ultimate3TStateTests, IsTerminal_OWins_ReturnsTrue)
{
    Ultimate3TState state;

    for (int i = 0; i < 9; i++)
    {
        state.setSpacePlayed(i/3, i % 3, player::o);
    }

    EXPECT_TRUE(state.isTerminalState());
}

TEST(Ultimate3TStateTests, IsTerminal_Draw_ReturnsTrue)
{
    Ultimate3TState state;

    for(int i = 0; i < 81; i++)
    {
        state.setSpacePlayed(i/9, i % 9, player::draw);
    }

    EXPECT_TRUE(state.isTerminalState());
}

TEST(Ultimate3TStateTests, IsTerminal_OngoingGame_ReturnsFalse)
{
    Ultimate3TState state;

    EXPECT_FALSE(state.isTerminalState());
}