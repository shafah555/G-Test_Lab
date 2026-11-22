#include <gtest/gtest.h>
#include "student_grade.h"

// 1. Total marks
TEST(StudentGradeTest, TotalMarks) {
    StudentGrade student;

    EXPECT_EQ(student.total(80, 70, 90), 240);
}

// 2. Normal marks
TEST(StudentGradeTest, NormalMarks) {
    StudentGrade student;

    EXPECT_EQ(student.total(80, 70, 90), 240);
    EXPECT_EQ(student.total(55, 65, 75), 195);
    EXPECT_DOUBLE_EQ(student.average(80, 70, 90), 80.0);
    EXPECT_EQ(student.grade(student.average(80, 70, 90)), 'A');
}

// 3. All marks zero
TEST(StudentGradeTest, ZeroMarks) {
    StudentGrade student;

    EXPECT_EQ(student.total(0, 0, 0), 0);
    EXPECT_DOUBLE_EQ(student.average(0, 0, 0), 0.0);
    EXPECT_EQ(student.grade(student.average(0, 0, 0)), 'F');
}

// 4. Full marks
TEST(StudentGradeTest, FullMarks) {
    StudentGrade student;

    EXPECT_EQ(student.total(100, 100, 100), 300);
    EXPECT_DOUBLE_EQ(student.average(100, 100, 100), 100.0);
    EXPECT_EQ(student.grade(student.average(100, 100, 100)), 'A');
}

// 5. Average calculation
TEST(StudentGradeTest, AverageMarks) {
    StudentGrade student;

    EXPECT_DOUBLE_EQ(
        student.average(80, 70, 90),
        80.0
    );
    EXPECT_DOUBLE_EQ(student.average(60, 60, 60), 60.0);
    // The average is not always a whole number: (80 + 80 + 81) / 3.0 = 80.333...
    // EXPECT_NEAR checks that two decimals are close (within 0.0001).
    EXPECT_NEAR(student.average(80, 80, 81), 80.3333333, 0.0001);
}

// 6. Grade calculation
TEST(StudentGradeTest, GradeCalculation) {
    StudentGrade student;

    EXPECT_EQ(student.grade(95.0), 'A');
    EXPECT_EQ(student.grade(75.0), 'B');
    EXPECT_EQ(student.grade(65.0), 'C');
    EXPECT_EQ(student.grade(55.0), 'D');
    EXPECT_EQ(student.grade(30.0), 'F');
}

// 7. Boundary values
TEST(StudentGradeTest, GradeBoundaryValues) {
    StudentGrade student;

    EXPECT_EQ(student.grade(80), 'A');
    EXPECT_EQ(student.grade(79), 'B');

    EXPECT_EQ(student.grade(70), 'B');
    EXPECT_EQ(student.grade(69), 'C');

    EXPECT_EQ(student.grade(60), 'C');
    EXPECT_EQ(student.grade(59), 'D');

    EXPECT_EQ(student.grade(50), 'D');
    EXPECT_EQ(student.grade(49), 'F');

    EXPECT_EQ(student.grade(0), 'F');
    EXPECT_EQ(student.grade(100), 'A');
}

// 8. Invalid marks
TEST(StudentGradeTest, InvalidMarks) {
    StudentGrade student;

    EXPECT_THROW(student.total(-1, 50, 50), std::invalid_argument);
    EXPECT_THROW(student.total(50, 101, 50), std::invalid_argument);
    EXPECT_THROW(student.total(50, 50, 120), std::invalid_argument);
    EXPECT_THROW(student.average(-10, 50, 50), std::invalid_argument);
    EXPECT_THROW(student.average(50, 50, 101), std::invalid_argument);
    EXPECT_THROW(student.grade(-1), std::invalid_argument);
    EXPECT_THROW(student.grade(100.5), std::invalid_argument);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
