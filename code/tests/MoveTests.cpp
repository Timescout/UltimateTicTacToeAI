/* Andrew Bergman
9-14-23
Tests for the Move struct
*/

#include "State.h"
#include <gtest/gtest.h>

TEST(MoveTests, ToBinary_Creates_CorrectBinary)
{
    move m(activeBoard::board1, 1);

    int binary = m.toBinary();

    EXPECT_EQ(binary, 0b00010001);
}

TEST(MoveTests, FromBinary_Creates_CorrectBinary)
{
    int binary = 0b00010001; //represents a move on board 1 space 1

    move m(binary);

    EXPECT_EQ(m.board, activeBoard::board1);
    EXPECT_EQ(m.space, 1);
}

TEST(MoveTests, FromBinary_InvalidBoard_ThrowsError)
{
    int binary = 0b11010000;

    EXPECT_THROW(move m(binary), std::out_of_range);
}

TEST(MoveTests, FromBinary_InvalidSpace_ThrowsError)
{
    int binary = 0b00001111;

    EXPECT_THROW(move m(binary), std::out_of_range);
}

TEST(MoveTests, FromBinary_AnyBoardValue_ThrowsError)
{
    int binary = 0b11110000;

    EXPECT_THROW(move m(binary), std::out_of_range);
}

///// Tests for ActiveBoard /////

// Yes, This was a bug.
TEST(ActiveBoardTests, Board8_Equals_8)
{
    EXPECT_EQ(activeBoard::board8, 8);
}