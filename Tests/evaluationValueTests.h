

#include "State.h"

#include "gtest.h"

TEST(EvaluationUnitTest, TwoOWinEvaluations_Compares_Correctly)
{
    evaluationValue soonerOWin;
    soonerOWin.gameValue = evaluation::oWin;
    soonerOWin.depth = -1; // wins this turn
    evaluationValue fartherOWin;
    fartherOWin.gameValue = evaluation::oWin;
    fartherOWin.depth = -3; // wins after two "O" moves.

    bool result = fartherOWin > soonerOWin; // since O wants to minimize the value, this needs to return false.

    EXPECT_FALSE(result);
}