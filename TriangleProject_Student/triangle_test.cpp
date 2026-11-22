#include <gtest/gtest.h>
#include <string>
#include "triangle.h"

// Example tests (given). Study them, then write the rest.
TEST(TriangleTest, EquilateralTriangle) {
    Triangle t;

    EXPECT_TRUE(t.isValid(5, 5, 5));
    EXPECT_EQ(t.getType(5, 5, 5), "Equilateral");
}

TEST(TriangleTest, InvalidTriangle) {
    Triangle t;

    EXPECT_FALSE(t.isValid(1, 2, 3));
}

// TODO: write at least 6 more tests (8 or more in total)
// TODO 1: IsoscelesTriangle      (5,5,3)  -> "Isosceles"
// TODO 2: ScaleneTriangle        (4,5,6)  -> "Scalene"
// TODO 3: InvalidTriangleSum     (1,1,5)  -> invalid
// TODO 4: EqualBoundaryCase      (2,2,4)  -> invalid (2 + 2 is not > 4)
// TODO 5: DifferentOrder         (3,5,4)  -> "Scalene"
// TODO 6: ZeroSide               (0,4,4)  -> invalid
// TODO 7: NegativeSide           (-2,4,5) -> invalid

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
