#include <gtest/gtest.h>
#include <climits>
#include <string>
#include "triangle.h"

// ---------- The 9 cases from the homework table ----------

// 1. Equilateral (5,5,5)
TEST(TriangleTest, EquilateralTriangle) {
    Triangle t;

    EXPECT_TRUE(t.isValid(5, 5, 5));
    EXPECT_EQ(t.getType(5, 5, 5), "Equilateral");
}

// 2. Isosceles (5,5,3)
TEST(TriangleTest, IsoscelesTriangle) {
    Triangle t;

    EXPECT_TRUE(t.isValid(5, 5, 3));
    EXPECT_EQ(t.getType(5, 5, 3), "Isosceles");
}

// 3. Scalene (4,5,6)
TEST(TriangleTest, ScaleneTriangle) {
    Triangle t;

    EXPECT_TRUE(t.isValid(4, 5, 6));
    EXPECT_EQ(t.getType(4, 5, 6), "Scalene");
}

// 4. Invalid (1,2,3): 1 + 2 == 3
TEST(TriangleTest, InvalidTriangle) {
    Triangle t;

    EXPECT_FALSE(t.isValid(1, 2, 3));
    EXPECT_EQ(t.getType(1, 2, 3), "Invalid");
}

// 5. Invalid (1,1,5): 1 + 1 < 5
TEST(TriangleTest, InvalidTriangleSumTooSmall) {
    Triangle t;

    EXPECT_FALSE(t.isValid(1, 1, 5));
    EXPECT_EQ(t.getType(1, 1, 5), "Invalid");
}

// 6. Equal boundary (2,2,4): 2 + 2 == 4 (degenerate, must be invalid)
TEST(TriangleTest, EqualBoundaryCase) {
    Triangle t;

    EXPECT_FALSE(t.isValid(2, 2, 4));
    EXPECT_EQ(t.getType(2, 2, 4), "Invalid");
}

// 7. Different order (3,5,4): same triangle in any order
TEST(TriangleTest, DifferentOrder) {
    Triangle t;

    EXPECT_TRUE(t.isValid(3, 5, 4));
    EXPECT_EQ(t.getType(3, 5, 4), "Scalene");
    EXPECT_EQ(t.getType(4, 3, 5), "Scalene");
    EXPECT_EQ(t.getType(5, 4, 3), "Scalene");
}

// 8. Zero side (0,4,4)
TEST(TriangleTest, ZeroSide) {
    Triangle t;

    EXPECT_FALSE(t.isValid(0, 4, 4));
    EXPECT_FALSE(t.isValid(4, 0, 4));
    EXPECT_FALSE(t.isValid(4, 4, 0));
    EXPECT_EQ(t.getType(0, 4, 4), "Invalid");
}

// 9. Negative side (-2,4,5)
TEST(TriangleTest, NegativeSide) {
    Triangle t;

    EXPECT_FALSE(t.isValid(-2, 4, 5));
    EXPECT_FALSE(t.isValid(4, -2, 5));
    EXPECT_FALSE(t.isValid(4, 5, -2));
    EXPECT_EQ(t.getType(-2, 4, 5), "Invalid");
}

// ---------- Extra tests (more boundary and order coverage) ----------

// Isosceles in every position of the equal pair
TEST(TriangleTest, IsoscelesAnyOrder) {
    Triangle t;

    EXPECT_EQ(t.getType(5, 5, 3), "Isosceles");
    EXPECT_EQ(t.getType(5, 3, 5), "Isosceles");
    EXPECT_EQ(t.getType(3, 5, 5), "Isosceles");
}

// Invalid in every position of the longest side (tests all three inequalities)
TEST(TriangleTest, InvalidAnyOrder) {
    Triangle t;

    EXPECT_FALSE(t.isValid(1, 2, 3));
    EXPECT_FALSE(t.isValid(1, 3, 2));
    EXPECT_FALSE(t.isValid(3, 1, 2));
    EXPECT_FALSE(t.isValid(2, 3, 1));
}

// Smallest valid triangle and "just valid" boundary (sum is exactly 1 more)
TEST(TriangleTest, SmallestValidTriangles) {
    Triangle t;

    EXPECT_TRUE(t.isValid(1, 1, 1));
    EXPECT_EQ(t.getType(1, 1, 1), "Equilateral");
    EXPECT_TRUE(t.isValid(2, 3, 4));   // 2 + 3 = 5 > 4
    EXPECT_TRUE(t.isValid(2, 2, 3));   // 2 + 2 = 4 > 3
    EXPECT_FALSE(t.isValid(2, 2, 4));  // 2 + 2 = 4, not > 4
}

// All sides zero or all negative
TEST(TriangleTest, AllZeroAndAllNegative) {
    Triangle t;

    EXPECT_FALSE(t.isValid(0, 0, 0));
    EXPECT_FALSE(t.isValid(-3, -3, -3));
    EXPECT_EQ(t.getType(0, 0, 0), "Invalid");
    EXPECT_EQ(t.getType(-3, -3, -3), "Invalid");
}

// Very large sides must not overflow
TEST(TriangleTest, LargeValuesNoOverflow) {
    Triangle t;

    EXPECT_TRUE(t.isValid(INT_MAX, INT_MAX, INT_MAX));
    EXPECT_EQ(t.getType(INT_MAX, INT_MAX, INT_MAX), "Equilateral");
    EXPECT_FALSE(t.isValid(INT_MAX, 1, 1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
