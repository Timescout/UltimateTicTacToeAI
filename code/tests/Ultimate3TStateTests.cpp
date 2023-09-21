/* Andrew Bergman
9-14-23
Tests for the Ultimate3TState class.
*/

#include "State.h"
#include <gtest/gtest.h>

// TEST(Ultimate3TTests, ) 
// {}

TEST(Ultimate3TStateTests, SetSpacePlayed_invalidSpace_ThrowsError)
{
    Ultimate3TState state;

    EXPECT_THROW(state.setSpacePlayed(0, 9, player::x), std::out_of_range);
}

TEST(Ultimate3TStateTests, FetSpacePlayed_HighBorderValues_ReturnValue)
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

    EXPECT_EQ(moves.size(), 72); // 81 - 9 = 72
}

TEST(Ultimate3TStateTests, GenerateMoves_AnyBoardWithOneMovePlayed_GeneratesCorrectMoves)
{
    Ultimate3TState state;
    state.setActiveBoard(activeBoard::anyBoard);
    state.setSpacePlayed(0, 0, player::x);

    std::vector<move> moves = state.generateMoves();

    EXPECT_EQ(moves.size(), 80);
}

