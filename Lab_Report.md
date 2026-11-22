# Lab Report — Unit Testing in C++ with Google Test (GTest)

| | |
|---|---|
| **Course** | Software Testing / Software Engineering |
| **Lab** | Lab 5 — Unit Testing with Google Test |
| **Student name** | ______________________ |
| **Student ID** | ______________________ |
| **Date** | ______________________ |
| **Environment** | Ubuntu Linux, g++ (C++17), Google Test 1.14.0 |

---

## 1. Objective

To learn how to verify C++ code automatically with Google Test by writing unit tests for normal, boundary and invalid inputs, observing a failing test, locating the bug, fixing it and re-running the tests.

## 2. Tools

* `g++` compiler, Google Test (`libgtest-dev`), `-pthread`
* Assertions used: `EXPECT_EQ`, `EXPECT_NE`, `EXPECT_TRUE`, `EXPECT_FALSE`, `EXPECT_DOUBLE_EQ`, `EXPECT_NEAR`, `EXPECT_THROW`

## 3. Project structure

```
GTest_Lab/
├── CalculatorProject/                 demo
├── StudentGradeProject_Student/       task (starter)
├── StudentGradeProject_Instructor/    task (solution)
├── TriangleProject_Student/           homework (starter)
├── TriangleProject_Instructor/        homework (solution)
└── outputs/                           real run outputs used below
```

## 4. Part A — Calculator (demonstration)

### 4.1 Code

`calculator.h`

```cpp
#ifndef CALCULATOR_H
#define CALCULATOR_H

class Calculator {
public:
    int add(int a, int b);
    int subtract(int a, int b);
    int multiply(int a, int b);
    double divide(double a, double b);
};

#endif
```

`calculator.cpp`

```cpp
#include "calculator.h"
#include <stdexcept>

int Calculator::add(int a, int b) {
    return a + b;
}

int Calculator::subtract(int a, int b) {
    return a - b;
}

int Calculator::multiply(int a, int b) {
    return a * b;
}

double Calculator::divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Cannot divide by zero");
    }

    return a / b;
}
```

`calculator_test.cpp`

```cpp
#include <gtest/gtest.h>
#include "calculator.h"

// 1. Addition
TEST(CalculatorTest, Addition) {
    Calculator calc;

    EXPECT_EQ(calc.add(2, 3), 5);
    EXPECT_EQ(calc.add(10, 20), 30);
    EXPECT_EQ(calc.add(-2, -3), -5);
}

// 2. Subtraction
TEST(CalculatorTest, Subtraction) {
    Calculator calc;

    EXPECT_EQ(calc.subtract(10, 5), 5);
    EXPECT_EQ(calc.subtract(5, 10), -5);
    EXPECT_EQ(calc.subtract(5, 5), 0);
}

// 3. Multiplication
TEST(CalculatorTest, Multiplication) {
    Calculator calc;

    EXPECT_EQ(calc.multiply(5, 4), 20);
    EXPECT_EQ(calc.multiply(-5, 4), -20);
    EXPECT_EQ(calc.multiply(10, 0), 0);
}

// 4. Division
TEST(CalculatorTest, Division) {
    Calculator calc;

    EXPECT_DOUBLE_EQ(calc.divide(10, 2), 5.0);
    EXPECT_DOUBLE_EQ(calc.divide(9, 3), 3.0);
    EXPECT_DOUBLE_EQ(calc.divide(-10, 2), -5.0);
}

// 5. Division by zero
TEST(CalculatorTest, DivisionByZero) {
    Calculator calc;

    EXPECT_THROW(
        calc.divide(10, 0),
        std::invalid_argument
    );
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

### 4.2 Build and run

```bash
g++ calculator.cpp calculator_test.cpp -lgtest -pthread -o calculator_test
./calculator_test
```

### 4.3 Output (all tests pass)

```text
[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 5 tests from CalculatorTest
[ RUN      ] CalculatorTest.Addition
[       OK ] CalculatorTest.Addition (0 ms)
[ RUN      ] CalculatorTest.Subtraction
[       OK ] CalculatorTest.Subtraction (0 ms)
[ RUN      ] CalculatorTest.Multiplication
[       OK ] CalculatorTest.Multiplication (0 ms)
[ RUN      ] CalculatorTest.Division
[       OK ] CalculatorTest.Division (0 ms)
[ RUN      ] CalculatorTest.DivisionByZero
[       OK ] CalculatorTest.DivisionByZero (0 ms)
[----------] 5 tests from CalculatorTest (0 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 5 tests.
```

### 4.4 Failed-test demonstration

Bug introduced: `return a * b;` was changed to `return a + b; // BUG` in `multiply()`.

Output:

```text
[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 5 tests from CalculatorTest
[ RUN      ] CalculatorTest.Addition
[       OK ] CalculatorTest.Addition (0 ms)
[ RUN      ] CalculatorTest.Subtraction
[       OK ] CalculatorTest.Subtraction (0 ms)
[ RUN      ] CalculatorTest.Multiplication
calculator_test.cpp:26: Failure
Expected equality of these values:
  calc.multiply(5, 4)
    Which is: 9
  20

calculator_test.cpp:27: Failure
Expected equality of these values:
  calc.multiply(-5, 4)
    Which is: -1
  -20

calculator_test.cpp:28: Failure
Expected equality of these values:
  calc.multiply(10, 0)
    Which is: 10
  0

[  FAILED  ] CalculatorTest.Multiplication (0 ms)
[ RUN      ] CalculatorTest.Division
[       OK ] CalculatorTest.Division (0 ms)
[ RUN      ] CalculatorTest.DivisionByZero
[       OK ] CalculatorTest.DivisionByZero (0 ms)
[----------] 5 tests from CalculatorTest (0 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 4 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] CalculatorTest.Multiplication

 1 FAILED TEST
```

**Analysis:** `CalculatorTest.Multiplication` failed on three lines (`multiply(5,4)` gave 9, `multiply(-5,4)` gave -1, `multiply(10,0)` gave 10). The other four tests still passed, so the failure points straight at `multiply()`. After restoring `a * b`, all 5 tests pass again.

## 5. Part B — StudentGrade task

### 5.1 Specification

| Average | Grade |
|---|---|
| 80–100 | A |
| 70–79 | B |
| 60–69 | C |
| 50–59 | D |
| 0–49 | F |

Marks outside 0–100 throw `std::invalid_argument`.

### 5.2 Implementation

```cpp
#ifndef STUDENT_GRADE_H
#define STUDENT_GRADE_H

class StudentGrade {
public:
    int total(int a, int b, int c);
    double average(int a, int b, int c);
    char grade(double average);
};

#endif
```

```cpp
#include "student_grade.h"
#include <stdexcept>

int StudentGrade::total(int a, int b, int c) {
    if (a < 0 || a > 100 ||
        b < 0 || b > 100 ||
        c < 0 || c > 100) {
        throw std::invalid_argument("Invalid marks");
    }

    return a + b + c;
}

double StudentGrade::average(int a, int b, int c) {
    if (a < 0 || a > 100 ||
        b < 0 || b > 100 ||
        c < 0 || c > 100) {
        throw std::invalid_argument("Invalid marks");
    }

    return (a + b + c) / 3.0;
}

char StudentGrade::grade(double average) {
    if (average < 0 || average > 100) {
        throw std::invalid_argument("Invalid average");
    }

    if (average >= 80)
        return 'A';
    else if (average >= 70)
        return 'B';
    else if (average >= 60)
        return 'C';
    else if (average >= 50)
        return 'D';
    else
        return 'F';
}
```

### 5.3 Test code

```cpp
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
```

### 5.4 Starter code (red stage, placeholders not yet implemented)

```text
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from StudentGradeTest
[ RUN      ] StudentGradeTest.TotalMarks
student_grade_test.cpp:11: Failure
Expected equality of these values:
  student.total(80, 70, 90)
    Which is: 0
  240

[  FAILED  ] StudentGradeTest.TotalMarks (0 ms)
[ RUN      ] StudentGradeTest.AverageMarks
student_grade_test.cpp:17: Failure
Expected equality of these values:
  student.average(80, 70, 90)
    Which is: 0
  80.0
    Which is: 80

[  FAILED  ] StudentGradeTest.AverageMarks (0 ms)
[----------] 2 tests from StudentGradeTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 0 tests.
[  FAILED  ] 2 tests, listed below:
[  FAILED  ] StudentGradeTest.TotalMarks
[  FAILED  ] StudentGradeTest.AverageMarks

 2 FAILED TESTS
```

### 5.5 Final output (green stage)

```text
[==========] Running 8 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 8 tests from StudentGradeTest
[ RUN      ] StudentGradeTest.TotalMarks
[       OK ] StudentGradeTest.TotalMarks (0 ms)
[ RUN      ] StudentGradeTest.NormalMarks
[       OK ] StudentGradeTest.NormalMarks (0 ms)
[ RUN      ] StudentGradeTest.ZeroMarks
[       OK ] StudentGradeTest.ZeroMarks (0 ms)
[ RUN      ] StudentGradeTest.FullMarks
[       OK ] StudentGradeTest.FullMarks (0 ms)
[ RUN      ] StudentGradeTest.AverageMarks
[       OK ] StudentGradeTest.AverageMarks (0 ms)
[ RUN      ] StudentGradeTest.GradeCalculation
[       OK ] StudentGradeTest.GradeCalculation (0 ms)
[ RUN      ] StudentGradeTest.GradeBoundaryValues
[       OK ] StudentGradeTest.GradeBoundaryValues (0 ms)
[ RUN      ] StudentGradeTest.InvalidMarks
[       OK ] StudentGradeTest.InvalidMarks (0 ms)
[----------] 8 tests from StudentGradeTest (0 ms total)

[----------] Global test environment tear-down
[==========] 8 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 8 tests.
```

### 5.6 Test coverage

| # | Test | Covers |
|---|---|---|
| 1 | `TotalMarks` | total of 80, 70, 90 |
| 2 | `NormalMarks` | total, average and grade together |
| 3 | `ZeroMarks` | 0, 0, 0 → total 0, avg 0.0, grade F |
| 4 | `FullMarks` | 100 ×3 → total 300, avg 100.0, grade A |
| 5 | `AverageMarks` | `EXPECT_DOUBLE_EQ` and `EXPECT_NEAR` |
| 6 | `GradeCalculation` | one value per grade |
| 7 | `GradeBoundaryValues` | 80/79, 70/69, 60/59, 50/49, 0, 100 |
| 8 | `InvalidMarks` | -1, 101, 120, -10 in every position, `grade(-1)`, `grade(100.5)` |

## 6. Part C — Homework: Unit Testing of the Triangle Program

### 6.1 Specification

`isValid(a,b,c)` is true only when all sides are > 0 and the sum of any two sides is **greater than** the third. `getType(a,b,c)` returns `Equilateral`, `Isosceles`, `Scalene` or `Invalid`.

### 6.2 Implementation

`triangle.h`

```cpp
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <string>

class Triangle {
public:
    // true if a, b, c are all positive and any two sides sum to more than the third
    bool isValid(int a, int b, int c);

    // "Equilateral", "Isosceles", "Scalene" or "Invalid"
    std::string getType(int a, int b, int c);
};

#endif
```

`triangle.cpp`

```cpp
#include "triangle.h"

bool Triangle::isValid(int a, int b, int c) {
    // Every side must be positive (rejects zero and negative sides)
    if (a <= 0 || b <= 0 || c <= 0) {
        return false;
    }

    // Triangle inequality: the sum of any two sides must be GREATER than the third.
    // long long avoids integer overflow for very large sides (e.g. INT_MAX).
    long long x = a, y = b, z = c;
    return (x + y > z) && (x + z > y) && (y + z > x);
}

std::string Triangle::getType(int a, int b, int c) {
    if (!isValid(a, b, c)) {
        return "Invalid";
    }

    if (a == b && b == c) {
        return "Equilateral";
    }
    if (a == b || b == c || a == c) {
        return "Isosceles";
    }
    return "Scalene";
}
```

### 6.3 Test table

| Test Case | Input | Expected | GTest name |
|---|---|---|---|
| Equilateral | (5,5,5) | Equilateral | `EquilateralTriangle` |
| Isosceles | (5,5,3) | Isosceles | `IsoscelesTriangle` |
| Scalene | (4,5,6) | Scalene | `ScaleneTriangle` |
| Invalid | (1,2,3) | Invalid | `InvalidTriangle` |
| Invalid | (1,1,5) | Invalid | `InvalidTriangleSumTooSmall` |
| Equal boundary | (2,2,4) | Invalid | `EqualBoundaryCase` |
| Different order | (3,5,4) | Scalene | `DifferentOrder` |
| Zero side | (0,4,4) | Invalid | `ZeroSide` |
| Negative side | (-2,4,5) | Invalid | `NegativeSide` |
| Extra | any order of isosceles / invalid | as above | `IsoscelesAnyOrder`, `InvalidAnyOrder` |
| Extra | (1,1,1), (2,3,4), (2,2,3) | valid | `SmallestValidTriangles` |
| Extra | all 0, all negative | Invalid | `AllZeroAndAllNegative` |
| Extra | `INT_MAX` sides (overflow) | valid / invalid | `LargeValuesNoOverflow` |

### 6.4 Test code

```cpp
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
```

### 6.5 Build and run

```bash
g++ triangle.cpp triangle_test.cpp -lgtest -pthread -o triangle_test
./triangle_test
```

### 6.6 Starter code (red stage)

```text
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from TriangleTest
[ RUN      ] TriangleTest.EquilateralTriangle
triangle_test.cpp:9: Failure
Value of: t.isValid(5, 5, 5)
  Actual: false
Expected: true

triangle_test.cpp:10: Failure
Expected equality of these values:
  t.getType(5, 5, 5)
    Which is: ""
  "Equilateral"

[  FAILED  ] TriangleTest.EquilateralTriangle (0 ms)
[ RUN      ] TriangleTest.InvalidTriangle
[       OK ] TriangleTest.InvalidTriangle (0 ms)
[----------] 2 tests from TriangleTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] TriangleTest.EquilateralTriangle

 1 FAILED TEST
...
```

### 6.7 Final output (all 14 tests pass)

```text
[==========] Running 14 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 14 tests from TriangleTest
[ RUN      ] TriangleTest.EquilateralTriangle
[       OK ] TriangleTest.EquilateralTriangle (0 ms)
[ RUN      ] TriangleTest.IsoscelesTriangle
[       OK ] TriangleTest.IsoscelesTriangle (0 ms)
[ RUN      ] TriangleTest.ScaleneTriangle
[       OK ] TriangleTest.ScaleneTriangle (0 ms)
[ RUN      ] TriangleTest.InvalidTriangle
[       OK ] TriangleTest.InvalidTriangle (0 ms)
[ RUN      ] TriangleTest.InvalidTriangleSumTooSmall
[       OK ] TriangleTest.InvalidTriangleSumTooSmall (0 ms)
[ RUN      ] TriangleTest.EqualBoundaryCase
[       OK ] TriangleTest.EqualBoundaryCase (0 ms)
[ RUN      ] TriangleTest.DifferentOrder
[       OK ] TriangleTest.DifferentOrder (0 ms)
[ RUN      ] TriangleTest.ZeroSide
[       OK ] TriangleTest.ZeroSide (0 ms)
[ RUN      ] TriangleTest.NegativeSide
[       OK ] TriangleTest.NegativeSide (0 ms)
[ RUN      ] TriangleTest.IsoscelesAnyOrder
[       OK ] TriangleTest.IsoscelesAnyOrder (0 ms)
[ RUN      ] TriangleTest.InvalidAnyOrder
[       OK ] TriangleTest.InvalidAnyOrder (0 ms)
[ RUN      ] TriangleTest.SmallestValidTriangles
[       OK ] TriangleTest.SmallestValidTriangles (0 ms)
[ RUN      ] TriangleTest.AllZeroAndAllNegative
[       OK ] TriangleTest.AllZeroAndAllNegative (0 ms)
[ RUN      ] TriangleTest.LargeValuesNoOverflow
[       OK ] TriangleTest.LargeValuesNoOverflow (0 ms)
[----------] 14 tests from TriangleTest (0 ms total)

[----------] Global test environment tear-down
[==========] 14 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 14 tests.
```

### 6.8 Bug-finding activity

**Bug introduced** in `triangle.cpp`:

```cpp
// correct
return (x + y > z) && (x + z > y) && (y + z > x);

// BUG: > replaced by >=
return (x + y >= z) && (x + z >= y) && (y + z >= x);
```

**Output with the bug:**

```text
[==========] Running 14 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 14 tests from TriangleTest
[ RUN      ] TriangleTest.EquilateralTriangle
[       OK ] TriangleTest.EquilateralTriangle (0 ms)
[ RUN      ] TriangleTest.IsoscelesTriangle
[       OK ] TriangleTest.IsoscelesTriangle (0 ms)
[ RUN      ] TriangleTest.ScaleneTriangle
[       OK ] TriangleTest.ScaleneTriangle (0 ms)
[ RUN      ] TriangleTest.InvalidTriangle
triangle_test.cpp:36: Failure
Value of: t.isValid(1, 2, 3)
  Actual: true
Expected: false

triangle_test.cpp:37: Failure
Expected equality of these values:
  t.getType(1, 2, 3)
    Which is: "Scalene"
  "Invalid"

[  FAILED  ] TriangleTest.InvalidTriangle (0 ms)
[ RUN      ] TriangleTest.InvalidTriangleSumTooSmall
[       OK ] TriangleTest.InvalidTriangleSumTooSmall (0 ms)
[ RUN      ] TriangleTest.EqualBoundaryCase
triangle_test.cpp:52: Failure
Value of: t.isValid(2, 2, 4)
  Actual: true
Expected: false

triangle_test.cpp:53: Failure
Expected equality of these values:
  t.getType(2, 2, 4)
    Which is: "Isosceles"
  "Invalid"

[  FAILED  ] TriangleTest.EqualBoundaryCase (0 ms)
[ RUN      ] TriangleTest.DifferentOrder
[       OK ] TriangleTest.DifferentOrder (0 ms)
[ RUN      ] TriangleTest.ZeroSide
[       OK ] TriangleTest.ZeroSide (0 ms)
[ RUN      ] TriangleTest.NegativeSide
[       OK ] TriangleTest.NegativeSide (0 ms)
[ RUN      ] TriangleTest.IsoscelesAnyOrder
[       OK ] TriangleTest.IsoscelesAnyOrder (0 ms)
[ RUN      ] TriangleTest.InvalidAnyOrder
triangle_test.cpp:101: Failure
Value of: t.isValid(1, 2, 3)
  Actual: true
Expected: false

triangle_test.cpp:102: Failure
Value of: t.isValid(1, 3, 2)
  Actual: true
Expected: false

triangle_test.cpp:103: Failure
Value of: t.isValid(3, 1, 2)
  Actual: true
Expected: false

triangle_test.cpp:104: Failure
Value of: t.isValid(2, 3, 1)
  Actual: true
Expected: false

[  FAILED  ] TriangleTest.InvalidAnyOrder (0 ms)
[ RUN      ] TriangleTest.SmallestValidTriangles
triangle_test.cpp:115: Failure
Value of: t.isValid(2, 2, 4)
  Actual: true
Expected: false

[  FAILED  ] TriangleTest.SmallestValidTriangles (0 ms)
[ RUN      ] TriangleTest.AllZeroAndAllNegative
[       OK ] TriangleTest.AllZeroAndAllNegative (0 ms)
[ RUN      ] TriangleTest.LargeValuesNoOverflow
[       OK ] TriangleTest.LargeValuesNoOverflow (0 ms)
[----------] 14 tests from TriangleTest (0 ms total)

[----------] Global test environment tear-down
[==========] 14 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 10 tests.
[  FAILED  ] 4 tests, listed below:
[  FAILED  ] TriangleTest.InvalidTriangle
[  FAILED  ] TriangleTest.EqualBoundaryCase
[  FAILED  ] TriangleTest.InvalidAnyOrder
[  FAILED  ] TriangleTest.SmallestValidTriangles

 4 FAILED TESTS
```

**Analysis**

| Item | Result |
|---|---|
| Tests that failed | `InvalidTriangle`, `EqualBoundaryCase`, `InvalidAnyOrder`, `SmallestValidTriangles` |
| Tests that still passed | 10 (e.g. `(1,1,5)` is still rejected because 1 + 1 is clearly less than 5) |
| Expected vs actual | `isValid(1,2,3)` expected `false`, actual `true`; `getType(1,2,3)` expected `"Invalid"`, actual `"Scalene"`; `isValid(2,2,4)` expected `false`, actual `true` |
| Defective statement | the three `>=` comparisons in `isValid()` |
| Which test detected it first | `InvalidTriangle` (1,2,3), and `EqualBoundaryCase` (2,2,4) |
| Fix | change `>=` back to `>` |
| After fix | all 14 tests pass (section 6.7) |

**Why boundary testing mattered:** `(1,1,5)` passes even with the bug because it is far from the limit. Only inputs where the sum is **exactly equal** to the third side (the degenerate triangles `(1,2,3)` and `(2,2,4)`) reveal it.

## 7. Summary of results

| Project | Tests | Result |
|---|---|---|
| Calculator | 5 | all passed |
| StudentGrade (solution) | 8 | all passed |
| StudentGrade (starter) | 2 | fail until implemented (intended) |
| Triangle (solution) | 14 | all passed |
| Triangle (starter) | 2 | 1 fails until implemented (intended; the placeholder `false` accidentally satisfies the invalid-triangle test) |
| Calculator with `multiply` bug | 5 | 1 failed (bug detected) |
| Triangle with `>=` bug | 14 | 4 failed (bug detected) |

## 8. Conclusion

Unit tests check each function automatically and repeatably. Good suites cover normal, zero, boundary and invalid inputs. Failed tests show the file, line, actual and expected values, which makes bugs quick to locate. Boundary-value tests found the `>=` defect that ordinary tests missed. The cycle write code → write tests → run → fix → run again was followed in every part.

## 9. Screenshots (paste here)

1. Calculator: all tests passed — _paste_
2. StudentGrade: all tests passed — _paste_
3. Triangle: all tests passed — _paste_
4. Triangle: failed test after bug — _paste_
