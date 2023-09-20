/* Andrew Bergman
9-14-23
Tests for the Ultimate3TState class.
*/

#include "State.h"
#include <gtest/gtest.h>

// TEST(Ultimate3TTests, ) 
// {}

TEST(Ultimate3TState, setSpacePlayed_invalidSpace_ThrowsError)
{
    Ultimate3TState state;

    EXPECT_THROW(state.setSpacePlayed(0, 9, player::x), std::out_of_range);
}