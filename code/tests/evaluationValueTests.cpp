
#include "State.h"
#include "gtest/gtest.h"

// Needed for parameterized tests
class EvaluationUnitTestTrueResult :
    public testing::TestWithParam<std::vector<evaluationValue>>
{};

// Test cases for The following Test
INSTANTIATE_TEST_SUITE_P(VariousTrueGreaterThanTests, EvaluationUnitTestTrueResult, testing::Values
(
    std::vector<evaluationValue>({evaluationValue(player::x, 0), evaluationValue(player::o, 0)}),
    std::vector<evaluationValue>({evaluationValue(player::x, 81), evaluationValue(player::draw, 81)}),
    std::vector<evaluationValue>({evaluationValue(player::draw, 0), evaluationValue(player::o, 0)}),
    std::vector<evaluationValue>({evaluationValue(player::draw, 0), evaluationValue(player::o, 0)}),
    std::vector<evaluationValue>({evaluationValue(player::o, 1), evaluationValue(player::o, 0)})
));
TEST_P(EvaluationUnitTestTrueResult, GreaterThan_FirstGreaterThanSecond_Returns_True)
{
    // Unpack values to be tested, see above
    std::vector<evaluationValue> values = GetParam();

    EXPECT_TRUE(values[0] > values[1]);
}

// Needed for parameterized tests
class EvaluationUnitTestFalseResult :
    public testing::TestWithParam<std::vector<evaluationValue>>
{};

// Test cases for The following Test
INSTANTIATE_TEST_SUITE_P(VariousFalseGreaterThanTests, EvaluationUnitTestFalseResult, testing::Values
(
    std::vector<evaluationValue>({evaluationValue(player::draw, 0), evaluationValue(player::draw, 0)}),
    std::vector<evaluationValue>({evaluationValue(player::o, 81), evaluationValue(player::draw, 81)}),
    std::vector<evaluationValue>({evaluationValue(player::draw, 81), evaluationValue(player::x, 81)}),
    std::vector<evaluationValue>({evaluationValue(player::o, 81), evaluationValue(player::x, 81)})
));
TEST_P(EvaluationUnitTestFalseResult, GreaterThan_FirstLessThanOrEqualTo_Returns_False)
{
    // Unpack values to be tested, see above
    std::vector<evaluationValue> values = GetParam();

    EXPECT_FALSE(values[0] > values[1]);
}

// Needed for parameterized tests
class EvaluationUnitTestThrowResult :
    public testing::TestWithParam<std::vector<evaluationValue>>
{};

// Test cases for The following Test
INSTANTIATE_TEST_SUITE_P(VariousInvalidGreaterThanTests, EvaluationUnitTestThrowResult, testing::Values
(
    std::vector<evaluationValue>({evaluationValue(player::neither, 0), (evaluationValue(player::x, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::x, 0), (evaluationValue(player::neither, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::x, 82), (evaluationValue(player::x, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::x, -1), (evaluationValue(player::x, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::x, 0), (evaluationValue(player::x, 82))}),
    std::vector<evaluationValue>({evaluationValue(player::x, 0), (evaluationValue(player::x, -1))}),
    std::vector<evaluationValue>({evaluationValue(player::neither, 0), (evaluationValue(player::neither, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::neither, 0), (evaluationValue(player::draw, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::neither, 0), (evaluationValue(player::o, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::o, 0), (evaluationValue(player::neither, 0))}),
    std::vector<evaluationValue>({evaluationValue(player::draw, 0), (evaluationValue(player::neither, 0))})
));
TEST_P(EvaluationUnitTestThrowResult, GreaterThan_EitherInvalid_Throws_InvalidArgument)
{
    // Unpack values to be tested, see above
    std::vector<evaluationValue> values = GetParam();
    
    EXPECT_THROW(values[0] > values[1], std::invalid_argument);
}