#include <gtest/gtest.h>
#include <limits>
#include <my_common_cpp_utils/MathUtils.h>
#include <set>

namespace utils
{

namespace
{
const size_t capacity = 10;
const int min = 0;
const int max = 9;
} // namespace

class NoRepeatableRandomsTest : public ::testing::Test
{
protected:
    NoRepeatableRandoms<int> noRepeatableRandoms;
    NoRepeatableRandomsTest() : noRepeatableRandoms(capacity, min, max) {}
};

TEST_F(NoRepeatableRandomsTest, InitiallyNotEmpty)
{
    EXPECT_TRUE(!noRepeatableRandoms.empty());
}

TEST_F(NoRepeatableRandomsTest, PullReducesSize)
{
    for (size_t i = 0; i < capacity; ++i)
        noRepeatableRandoms.pull();
    EXPECT_TRUE(noRepeatableRandoms.empty());
}

TEST_F(NoRepeatableRandomsTest, PullReturnsUniqueNumbersInRange)
{
    std::set<int> numbers;
    while (!noRepeatableRandoms.empty())
    {
        int num = noRepeatableRandoms.pull();
        EXPECT_GE(num, min);
        EXPECT_LE(num, max);
        numbers.insert(num);
    }

    EXPECT_EQ(numbers.size(), capacity);
}

TEST(Math, RoundStep)
{
    EXPECT_EQ(roundStep(85, 10), 80);
    EXPECT_EQ(roundStep(86, 10), 80);
    EXPECT_EQ(roundStep(0, 10), 0);
    EXPECT_EQ(roundStep(100, 10), 100);
}

} // namespace utils