#include "RdTest.h"

TEST(ExpectEq, Int)
{
    int x = 7;
    int y = 4;
    EXPECT_EQ(1, 1);
    EXPECT_EQ(7, x);
    EXPECT_EQ(x, 7);
    EXPECT_EQ(2+2, 4);
    EXPECT_EQ(6, 2*3);
    EXPECT_EQ(3*4, 5+7);
    EXPECT_EQ(y+3, x);
    EXPECT_EQ(x-1, y+2);
}

TEST(ExpectEq, ShouldFail)
{
    int x = 7;
    EXPECT_EQ(1, 2);
    EXPECT_EQ(x, 5+3);
    EXPECT_EQ(5+3, x+2);
}

int main(int argc, char * argv[])
{
    return RUN_ALL_TESTS();
}
