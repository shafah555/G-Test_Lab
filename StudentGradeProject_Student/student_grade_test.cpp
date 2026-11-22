#include <gtest/gtest.h>
#include <stdexcept>
#include "student_grade.h"

// ------------------------------------------------------------
// Example tests (given to you). Study them, then write the rest.
// ------------------------------------------------------------
TEST(StudentGradeTest, TotalMarks) {
    StudentGrade student;

    EXPECT_EQ(student.total(80, 70, 90), 240);
}

TEST(StudentGradeTest, AverageMarks) {
    StudentGrade student;

    EXPECT_DOUBLE_EQ(
        student.average(80, 70, 90),
        80.0
    );
}

// ------------------------------------------------------------
// TODO: write these tests yourself
// ------------------------------------------------------------

// TODO 1: NormalMarks   - use marks like 80, 70, 90 (total, average and grade)
// TODO 2: ZeroMarks     - 0, 0, 0
// TODO 3: FullMarks     - 100, 100, 100
// TODO 4: GradeCalculation   - one normal value for each grade A, B, C, D, F
// TODO 5: GradeBoundaryValues - 80/79, 70/69, 60/59, 50/49
// TODO 6: InvalidMarks  - use EXPECT_THROW with std::invalid_argument
//                         (try -1, 101, 120 in total(), average() and grade())

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
