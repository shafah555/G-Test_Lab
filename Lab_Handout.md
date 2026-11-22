# 1. Lab Title

# Unit Testing in C++ with Google Test (GTest)

**Course:** Software Testing / Software Engineering
**Level:** Undergraduate (beginner friendly)
**Duration:** about 2 to 3 hours
**Tools:** Linux/Ubuntu, `g++`, Google Test (and optionally CMake)

---

# 2. Learning Objectives

After completing this lab, students will be able to:

1. Understand the purpose of unit testing.
2. Create simple C++ unit tests using Google Test.
3. Use `EXPECT_EQ` for integer comparisons.
4. Use `EXPECT_DOUBLE_EQ` for floating-point comparisons.
5. Use `EXPECT_THROW` for exception testing.
6. Test normal and edge cases.
7. Perform boundary value testing.
8. Identify bugs using failed unit tests.
9. Fix implementation errors and rerun tests.
10. Understand the difference between `EXPECT` and `ASSERT`.

---

# 3. Introduction to Unit Testing

## 3.1 What is a unit test?

A **unit** is the smallest testable part of a program, usually one function or one class.
A **unit test** is a small piece of code that:

1. calls the function with known input,
2. compares the **actual result** with the **expected result**, and
3. reports **pass** or **fail**.

## 3.2 Why do we write unit tests?

| Reason | Explanation |
|---|---|
| Find bugs early | A bug found today is cheaper to fix than a bug found after release. |
| Safe changes | After changing code, run the tests again. If they pass, nothing broke. |
| Documentation | Tests show how a function is supposed to behave. |
| Confidence | You can trust your code because you have checked it. |

## 3.3 What is Google Test?

Google Test (GTest) is a free C++ testing library from Google. It gives us:

* `TEST(...)` to define a test,
* assertions like `EXPECT_EQ` to check results,
* a runner that runs all tests and prints a report.

## 3.4 Basic shape of a GTest test

```cpp
TEST(TestSuiteName, TestName) {
    // 1. Arrange - create objects
    // 2. Act     - call the function
    // 3. Assert  - check the result with EXPECT_...
}
```

* `TestSuiteName` groups related tests (for example `CalculatorTest`).
* `TestName` describes what this test checks (for example `Addition`).

## 3.5 The software testing cycle

```
Write Code
    ↓
Write Tests
    ↓
Run Tests
    ↓
Pass?
   /   \
 Yes    No
  ↓      ↓
Continue Find the Bug
          ↓
       Fix Code
          ↓
       Run Again
```

You will follow this cycle many times during this lab.

---

# 4. Teacher Demonstration — Calculator

> **IMPORTANT:** The Calculator is **only the teacher's demonstration.**
> It is **not** your assignment. Your assignment starts at **Section 12 (StudentGrade).**

## Problem scenario

A software company is developing a simple **Calculator** module for an educational application.
The calculator must perform:

* Addition
* Subtraction
* Multiplication
* Division

Before joining the calculator to the main application, the team wants to check each function separately using Google Test.

The team decides to test:

1. Positive numbers
2. Negative numbers
3. Zero
4. Boundary / edge cases
5. Division behavior

---

# 5. Calculator Project Structure

```
CalculatorProject/
│
├── calculator.h
├── calculator.cpp
└── calculator_test.cpp
```

| File | Purpose |
|---|---|
| `calculator.h` | Declares the `Calculator` class (what it can do). |
| `calculator.cpp` | Implements the functions (how it does it). |
| `calculator_test.cpp` | Contains the GTest tests. |

---

# 6. Calculator Implementation

### `calculator.h`

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

`#ifndef / #define / #endif` is a **header guard**. It stops the header from being included twice.

### `calculator.cpp`

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

Notice that `divide` **throws an exception** when `b` is zero. We must test this behavior too.

---

# 7. Calculator GTest Tests

### `calculator_test.cpp`

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

### What the tests cover

| Test | Idea being tested |
|---|---|
| `Addition` | positive + positive, and negative + negative |
| `Subtraction` | normal, negative result, and zero result |
| `Multiplication` | positive, negative, and multiply by zero |
| `Division` | normal results, and a negative number |
| `DivisionByZero` | the error (exception) case |

### The GTest runner (`main`)

```cpp
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

1. `InitGoogleTest` starts Google Test and reads command-line options.
2. `RUN_ALL_TESTS()` runs every `TEST` in the program and returns `0` if all passed.

> If you link with `-lgtest_main`, GTest supplies this `main` for you. In this lab we write it ourselves so you can see how it works.

---

# 8. Explanation of GTest Assertions

An **assertion** is a line that checks whether something is true. If it is false, the test fails.

## 8.1 `EXPECT_EQ` — for integers

```cpp
EXPECT_EQ(calc.add(2, 3), 5);
```

Checks that the two values are **exactly equal**. Integers are stored exactly, so exact comparison is safe and correct.

## 8.2 `EXPECT_DOUBLE_EQ` — for floating-point numbers

```cpp
EXPECT_DOUBLE_EQ(calc.divide(10, 2), 5.0);
```

Computers store `double` values in binary, and many decimal numbers cannot be stored exactly.
For example, `0.1 + 0.2` may give `0.30000000000000004` instead of `0.3`.

So comparing doubles with `==` (or `EXPECT_EQ`) can fail even when the code is correct.
`EXPECT_DOUBLE_EQ` accepts values that are **almost equal** (within 4 units in the last place), which is the correct way to compare doubles.

| Data type | Use |
|---|---|
| `int`, `char`, `bool` | `EXPECT_EQ` |
| `double` | `EXPECT_DOUBLE_EQ` |
| `double` with your own tolerance | `EXPECT_NEAR(a, b, tolerance)` |

## 8.3 `EXPECT_THROW` — for exceptions

```cpp
EXPECT_THROW(calc.divide(10, 0), std::invalid_argument);
```

Checks that the code **throws** an exception of the given type.

* If the exception is thrown → **pass**.
* If no exception is thrown → **fail**.
* If a different exception type is thrown → **fail**.

We cannot test this with `EXPECT_EQ`, because when an exception is thrown there is no return value to compare.

## 8.4 Other useful assertions

| Assertion | Meaning | Example |
|---|---|---|
| `EXPECT_TRUE(cond)` | `cond` must be true | `EXPECT_TRUE(calc.add(1, 1) == 2);` |
| `EXPECT_FALSE(cond)` | `cond` must be false | `EXPECT_FALSE(calc.add(1, 1) == 3);` |
| `EXPECT_NE(a, b)` | `a` must **not** equal `b` | `EXPECT_NE(calc.add(1, 1), 3);` |
| `EXPECT_DOUBLE_EQ(a, b)` | two doubles almost equal | `EXPECT_DOUBLE_EQ(calc.divide(1, 4), 0.25);` |
| `EXPECT_THROW(stmt, type)` | `stmt` throws `type` | `EXPECT_THROW(calc.divide(1, 0), std::invalid_argument);` |

---

# 9. Successful Test Output

When every test passes, you see this:

```
[==========] Running 5 tests from 1 test suite.
[----------] 5 tests from CalculatorTest
[ RUN      ] CalculatorTest.Addition
[       OK ] CalculatorTest.Addition
[ RUN      ] CalculatorTest.Subtraction
[       OK ] CalculatorTest.Subtraction
[ RUN      ] CalculatorTest.Multiplication
[       OK ] CalculatorTest.Multiplication
[ RUN      ] CalculatorTest.Division
[       OK ] CalculatorTest.Division
[ RUN      ] CalculatorTest.DivisionByZero
[       OK ] CalculatorTest.DivisionByZero
[----------] 5 tests from CalculatorTest
[==========] 5 tests from 1 test suite ran.
[  PASSED  ] 5 tests.
```

> Your real output also shows extra lines such as `Global test environment set-up` and the time in milliseconds, for example `(0 ms)`. This is normal.

How to read it:

| Marker | Meaning |
|---|---|
| `[ RUN      ]` | a test is starting |
| `[       OK ]` | the test passed |
| `[  FAILED  ]` | the test failed |
| `[  PASSED  ]` | summary: all tests passed |

---

# 10. Failed Test Demonstration

A test that never fails is not proven useful. Let us **add a bug on purpose** and see the tests catch it.

### Step 1 — Introduce the bug

In `calculator.cpp`, change `multiply`:

```cpp
int Calculator::multiply(int a, int b) {
    return a + b;   // BUG
}
```

Save the file and rebuild.

### Step 2 — Run the tests

The line

```cpp
EXPECT_EQ(calc.multiply(5, 4), 20);
```

now fails because:

```
Expected = 20
Actual   = 9      (the bug computes 5 + 4)
```

### Step 3 — Read the failure message

Real output:

```
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

[  FAILED  ] CalculatorTest.Multiplication
...
[  PASSED  ] 4 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] CalculatorTest.Multiplication
```

What it tells us:

1. **Which file and line** failed (`calculator_test.cpp:26`).
2. **What was computed** (`Which is: 9`).
3. **What was expected** (`20`).
4. **Only `Multiplication` failed**, so the bug is in `multiply`. The other four tests still pass.

Notice that all **three** lines in the test were reported. That is because `EXPECT_EQ` does not stop the test (see Section 11).

### Step 4 — Fix and run again

Change the code back to:

```cpp
int Calculator::multiply(int a, int b) {
    return a * b;
}
```

Run the tests again. All 5 tests pass. This is the testing cycle from Section 3.5.

---

# 11. EXPECT vs ASSERT

| | `EXPECT_*` | `ASSERT_*` |
|---|---|---|
| On failure | Failure is **reported**, the test **continues** | Failure is **reported**, the test **stops immediately** |
| Also called | non-fatal failure | fatal failure |
| Example | `EXPECT_EQ(a, b);` | `ASSERT_EQ(a, b);` |

```cpp
TEST(AssertDemo, ExpectContinues) {
    Calculator c;
    EXPECT_EQ(c.multiply(5, 4), 21);   // fails, but test goes on
    EXPECT_EQ(c.multiply(2, 2), 5);    // also checked and also fails
}

TEST(AssertDemo, AssertStops) {
    Calculator c;
    ASSERT_EQ(c.multiply(5, 4), 21);   // fails, test STOPS here
    EXPECT_EQ(c.multiply(2, 2), 5);    // never runs
}
```

Try it yourself: add these two tests temporarily to `calculator_test.cpp`. The first test reports two failure messages, but the second test reports only one, because `ASSERT_EQ` stops the test at the first failure. Remove them afterwards.

**When to use which?**

* Use **`EXPECT`** by default. You see *all* problems in one run.
* Use **`ASSERT`** when the rest of the test makes no sense after a failure (for example, a pointer is `nullptr` and the next line would use it).

---

# 12. Student Lab Task — StudentGrade

> From here on, **this is your work.**

## Problem scenario

A school wants to develop a simple **Student Grade Calculator**.
The system accepts marks for **three subjects** and must:

* Calculate the **total** marks
* Calculate the **average** marks
* Determine the student's **grade**

You must **implement the class** and **write the GTest test cases**.

## The class

```cpp
class StudentGrade {
public:
    int total(int a, int b, int c);
    double average(int a, int b, int c);
    char grade(double average);
};
```

## Grading system

| Average Marks | Grade |
|---|---|
| 80 – 100 | A |
| 70 – 79 | B |
| 60 – 69 | C |
| 50 – 59 | D |
| 0 – 49 | F |

## Rules

* Marks must be between **0 and 100** (inclusive).
* Invalid marks must throw **`std::invalid_argument`**.
* Invalid examples: `-10`, `-1`, `101`, `120`, and so on.

---

# 13. Student Requirements

## 13.1 What you are GIVEN

| Item | Where |
|---|---|
| `student_grade.h` (class declaration) | `StudentGradeProject_Student/student_grade.h` |
| `student_grade.cpp` (empty placeholders with `TODO` comments) | `StudentGradeProject_Student/student_grade.cpp` |
| `student_grade_test.cpp` with 2 example tests and the `main()` runner | `StudentGradeProject_Student/student_grade_test.cpp` |
| The Calculator demo to learn from | `CalculatorProject/` |

## 13.2 What you must IMPLEMENT

In `student_grade.cpp`:

1. `int total(int a, int b, int c)` — return `a + b + c`.
2. `double average(int a, int b, int c)` — return the average as a decimal number.
3. `char grade(double average)` — return `'A'`, `'B'`, `'C'`, `'D'` or `'F'`.
4. Throw `std::invalid_argument` for any mark outside 0–100.
5. Also throw `std::invalid_argument` in `grade()` when the average is outside 0–100.

## 13.3 What you must WRITE (tests)

In `student_grade_test.cpp`, complete the tests listed in Section 15.

## 13.4 Expected behavior

| Call | Expected result |
|---|---|
| `total(80, 70, 90)` | `240` |
| `average(80, 70, 90)` | `80.0` |
| `grade(80.0)` | `'A'` |
| `total(0, 0, 0)` | `0` |
| `total(100, 100, 100)` | `300` |
| `total(-1, 50, 50)` | throws `std::invalid_argument` |
| `average(50, 101, 50)` | throws `std::invalid_argument` |
| `grade(-5)` | throws `std::invalid_argument` |

## 13.5 Testing requirements

* Use `EXPECT_EQ` for `int` and `char` values.
* Use `EXPECT_DOUBLE_EQ` for `double` values.
* Use `EXPECT_THROW` with `std::invalid_argument` for invalid input.
* Every test must have a meaningful name.
* All tests must pass at the end.

## 13.6 Suggested workflow

1. Compile the starter code. The two given tests will **fail** (the placeholders return wrong values). This is correct. This is the **"red"** stage.
2. Implement `total()` first. Run the tests.
3. Implement `average()`. Run the tests.
4. Implement `grade()`. Run the tests.
5. Add validation (exceptions). Run the tests.
6. Write the remaining tests from Section 15. Run the tests again.
7. Introduce a bug on purpose, watch a test fail, then fix it (Section 21).

---

# 14. Student Project Structure

```
StudentGradeProject/
│
├── student_grade.h
├── student_grade.cpp
└── student_grade_test.cpp
```

(In this package the starter version is in `StudentGradeProject_Student/`.)

---

# 15. Student Test Requirements

Write tests for **all** of the following.

| # | Test name (suggested) | What to test | Example data |
|---|---|---|---|
| 1 | `TotalMarks` | total of three marks | `80, 70, 90 → 240` *(given)* |
| 2 | `NormalMarks` | total, average and grade for normal marks | `80, 70, 90` |
| 3 | `ZeroMarks` | all marks zero | `0, 0, 0` → total 0, average 0.0, grade `'F'` |
| 4 | `FullMarks` | all marks full | `100, 100, 100` → total 300, average 100.0, grade `'A'` |
| 5 | `AverageMarks` | average calculation | `80, 70, 90 → 80.0` *(given)* |
| 6 | `GradeCalculation` | one normal value per grade | A, B, C, D, F |
| 7 | `GradeBoundaryValues` | the edges between grades | see table below |
| 8 | `InvalidMarks` | invalid input throws | `-1`, `101`, `120`, `-10` |

### Given example tests

```cpp
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
```

### Boundary values to test

**Boundary value testing** means testing the values right at the edge of a rule, because bugs often hide there (for example writing `>` instead of `>=`).

| Input | Expected grade | Why it matters |
|---|---|---|
| 80 | A | lowest A |
| 79 | B | highest B |
| 70 | B | lowest B |
| 69 | C | highest C |
| 60 | C | lowest C |
| 59 | D | highest D |
| 50 | D | lowest D |
| 49 | F | highest F |

### Invalid input

Use `EXPECT_THROW` with `std::invalid_argument`, for example:

```cpp
EXPECT_THROW(student.total(-1, 50, 50), std::invalid_argument);
```

Test the invalid value in **each** position (first, second and third mark) and test `total()`, `average()` and `grade()`.

---

# 16. Hints for Students

1. **Start small.** Make one test pass, then the next.
2. **Use `3.0`, not `3`** when averaging:
   ```cpp
   (a + b + c) / 3     // integer division! 80+70+91 = 241 / 3 = 80 (loses decimals)
   (a + b + c) / 3.0   // correct: 80.3333...
   ```
3. **Write a validation check** with `||` ("or"):
   ```cpp
   if (a < 0 || a > 100 || b < 0 || b > 100 || c < 0 || c > 100) {
       throw std::invalid_argument("Invalid marks");
   }
   ```
4. **Order of `if` statements in `grade()`:** check from the highest grade down (`>= 80`, then `>= 70`, ...). Then you do not need an upper limit in each `if`.
5. **Include the right headers:** `#include <stdexcept>` in your `.cpp` file.
6. **Use `EXPECT_EQ` for `char`:** `EXPECT_EQ(student.grade(80), 'A');`
7. **Read the failure message.** It tells you the file, the line, the actual value and the expected value.
8. **Look at the Calculator demo.** Every pattern you need is there.
9. **Do not skip the failed-test step.** A test is only trustworthy if you have seen it fail for the right reason.

---

# 17. Instructor Reference Solution

> **INSTRUCTOR ONLY.**
> Do not share Sections 17 to 21 with students until after they have attempted the task.
> The code is in `StudentGradeProject_Instructor/`.

---

# 18. Complete StudentGrade Implementation

### `student_grade.h`

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

### `student_grade.cpp`

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

### Notes for the instructor

* The same validation appears in `total()` and `average()`. For beginners this duplication is acceptable. A good follow-up discussion is moving it into one private helper function.
* A value such as `79.5` returns `'B'` (it is below 80). The assignment table is written for whole numbers, so you may want to discuss this with students.

---

# 19. Complete GTest Test File

### `student_grade_test.cpp`

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

### Marking guide (suggested, 100 marks)

| Item | Marks |
|---|---|
| `student_grade.cpp` compiles and works | 25 |
| Tests for normal, zero and full marks | 15 |
| Average and grade tests (correct use of `EXPECT_DOUBLE_EQ`, `EXPECT_EQ`) | 15 |
| Boundary value tests (all 8 values) | 20 |
| Invalid input tests (`EXPECT_THROW`) | 15 |
| Failed-test demonstration (screenshot + short explanation) | 10 |

---

# 20. Expected Output

```
[==========] Running 8 tests from 1 test suite.
[----------] 8 tests from StudentGradeTest
[ RUN      ] StudentGradeTest.TotalMarks
[       OK ] StudentGradeTest.TotalMarks
[ RUN      ] StudentGradeTest.NormalMarks
[       OK ] StudentGradeTest.NormalMarks
[ RUN      ] StudentGradeTest.ZeroMarks
[       OK ] StudentGradeTest.ZeroMarks
[ RUN      ] StudentGradeTest.FullMarks
[       OK ] StudentGradeTest.FullMarks
[ RUN      ] StudentGradeTest.AverageMarks
[       OK ] StudentGradeTest.AverageMarks
[ RUN      ] StudentGradeTest.GradeCalculation
[       OK ] StudentGradeTest.GradeCalculation
[ RUN      ] StudentGradeTest.GradeBoundaryValues
[       OK ] StudentGradeTest.GradeBoundaryValues
[ RUN      ] StudentGradeTest.InvalidMarks
[       OK ] StudentGradeTest.InvalidMarks
[----------] 8 tests from StudentGradeTest
[==========] 8 tests from 1 test suite ran.
[  PASSED  ] 8 tests.
```

If a student writes a different number of tests, their number (`8`) will differ. That is fine as long as all required areas are covered.

---

# 21. Failed Test Demonstration

Students should do this once, to **see** a failing test and to learn that tests really catch bugs.

### Step 1 — Introduce the bug

```cpp
double StudentGrade::average(int a, int b, int c) {
    return (a + b + c) / 2.0;  // BUG
}
```

Keep the validation code and change only the divisor. Save the file and rebuild.

### Step 2 — Run the test

```cpp
EXPECT_DOUBLE_EQ(
    student.average(80, 70, 90),
    80.0
);
```

The test **fails** because:

```
Expected = 80.0
Actual   = 120.0     (240 / 2.0 instead of 240 / 3.0)
```

Real output:

```
student_grade_test.cpp:43: Failure
Expected equality of these values:
  student.average(80, 70, 90)
    Which is: 120
  80.0
    Which is: 80
```

> **Why do other tests fail too?** With this bug, `average(100, 100, 100)` returns `150`. Then `grade(150)` throws `"Invalid average"`, so `FullMarks` and `NormalMarks` also fail. One bug can cause several failing tests. Always start with the **first** failure and look for the root cause. This is a good point to discuss with students.

### Step 3 — Fix the code

```cpp
double StudentGrade::average(int a, int b, int c) {
    return (a + b + c) / 3.0;   // correct
}
```

### Step 4 — Run again

All tests pass again:

```
[  PASSED  ] 8 tests.
```

---

# 22. Compilation and Execution

## 22.1 Install Google Test (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install g++ libgtest-dev
```

On recent Ubuntu versions this is enough. On some older versions `libgtest-dev` only provides the source code. If the linker later says `cannot find -lgtest`, build the library once:

```bash
sudo apt install cmake
cd /usr/src/googletest
sudo cmake -S . -B build
sudo cmake --build build
sudo cp build/lib/*.a /usr/lib
```

## 22.2 Compile and run — Calculator demo

```bash
cd CalculatorProject
g++ calculator.cpp calculator_test.cpp -lgtest -pthread -o calculator_test
./calculator_test
```

## 22.3 Compile and run — StudentGrade

```bash
cd StudentGradeProject_Student        # students
# or
cd StudentGradeProject_Instructor     # instructor solution

g++ student_grade.cpp student_grade_test.cpp \
    -lgtest -pthread \
    -o student_grade_test

./student_grade_test
```

### What each part of the command means

| Part | Meaning |
|---|---|
| `g++` | the C++ compiler |
| `student_grade.cpp student_grade_test.cpp` | the source files to compile together |
| `-lgtest` | link the Google Test library |
| `-lgtest_main` | *(not needed here)* links GTest's own `main()`. Our test files already contain `main()`, so we leave it out. If you delete your `main()`, add `-lgtest_main` instead. |
| `-pthread` | Google Test uses threads, so this is required |
| `-o student_grade_test` | name of the output program |

> The exact command may vary depending on how Google Test is installed or configured on your computer.
> If you installed GTest in a custom folder, add `-I/path/to/include -L/path/to/lib`.

## 22.4 Useful run options

```bash
./student_grade_test --gtest_filter=StudentGradeTest.InvalidMarks   # run one test
./student_grade_test --gtest_list_tests                              # list all tests
./student_grade_test --gtest_color=yes                               # colored output
```

## 22.5 Optional: CMake (modern approach)

CMake is not included in this package. If you want to try it, create a file named `CMakeLists.txt` inside the project folder with the content below, then build with:

```bash
cmake -S . -B build
cmake --build build
./build/student_grade_test
# or:
cd build && ctest --output-on-failure
```

`CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(StudentGradeProject CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_executable(student_grade_test student_grade.cpp student_grade_test.cpp)
target_link_libraries(student_grade_test GTest::gtest)

enable_testing()
add_test(NAME StudentGradeTests COMMAND student_grade_test)
```

CMake is optional. The `g++` command above is enough to complete the lab.

## 22.6 Common problems

| Problem | Likely cause / fix |
|---|---|
| `gtest/gtest.h: No such file` | Google Test not installed. Install `libgtest-dev`. |
| `cannot find -lgtest` | GTest library not built. See Section 22.1. |
| `undefined reference to pthread_create` | You forgot `-pthread`. |
| `undefined reference to StudentGrade::total` | You forgot to compile `student_grade.cpp` in the same command. |
| `multiple definition of main` | Two `main()` functions. Remove `-lgtest_main` or your own `main()`. |
| Test fails with `unknown file: C++ exception` | Your code threw an exception that the test did not expect. Check your validation. |

---

# 23. Final Summary

In this lab you learned that:

1. A **unit test** checks one small part of a program automatically.
2. A GTest test is written with `TEST(Suite, Name) { ... }`.
3. Use **`EXPECT_EQ`** for integers and characters, **`EXPECT_DOUBLE_EQ`** for decimals, and **`EXPECT_THROW`** for errors.
4. Good tests cover **normal values, zero, full/maximum values, boundary values and invalid input**.
5. **Boundary value testing** (79 vs 80, 49 vs 50, and so on) finds many real bugs.
6. A **failed test** shows file, line, actual value and expected value, so it helps you find the bug.
7. The cycle is **write code → write tests → run → fix → run again**.
8. **`EXPECT`** continues after a failure, **`ASSERT`** stops the test.

### Quick reference card

| I want to check... | Use |
|---|---|
| two integers/chars are equal | `EXPECT_EQ(actual, expected)` |
| two integers/chars are different | `EXPECT_NE(a, b)` |
| two doubles are equal | `EXPECT_DOUBLE_EQ(actual, expected)` |
| two doubles are close | `EXPECT_NEAR(a, b, tolerance)` |
| a condition is true / false | `EXPECT_TRUE(c)` / `EXPECT_FALSE(c)` |
| a function throws | `EXPECT_THROW(stmt, ExceptionType)` |
| a function does not throw | `EXPECT_NO_THROW(stmt)` |
| stop the test on failure | use `ASSERT_...` instead of `EXPECT_...` |

---

# 24. Optional Challenge Tasks

These tasks are **not required**. Try them if you finish early.

1. **Add a fourth subject.** Create `total4(a, b, c, d)` and `average4(a, b, c, d)`. Update the tests.
2. **Add GPA calculation.** Create `double gpa(double average)` that converts the average to a 4.0 scale (for example A = 4.0, B = 3.0, C = 2.0, D = 1.0, F = 0.0).
3. **Add plus/minus grades** such as `A+`, `A`, `A-`. (Hint: `char` is not enough; return `std::string`.)
4. **Add a pass/fail function.** `bool isPass(double average)` (pass when average ≥ 50). Test it with `EXPECT_TRUE` and `EXPECT_FALSE`.
5. **Add more invalid-input tests.** Try `INT_MAX`, `INT_MIN`, and invalid values in every parameter position.
6. **Use `ASSERT_EQ`.** Rewrite one test using `ASSERT_EQ` and explain how its behavior differs.
7. **Use CMake.** Build the project with CMake and run it with `ctest`.
8. **Create a test fixture with `TEST_F`.** Create the `StudentGrade` object once for all tests:

   ```cpp
   class StudentGradeFixture : public ::testing::Test {
   protected:
       StudentGrade student;   // created fresh before every test
   };

   TEST_F(StudentGradeFixture, TotalMarks) {
       EXPECT_EQ(student.total(80, 70, 90), 240);
   }
   ```

# 25. Homework — Unit Testing of Triangle Program

> Independent task. Reuse everything from the Calculator demo and the StudentGrade task.
> Starter code: `TriangleProject_Student/`. Instructor solution: `TriangleProject_Instructor/`.

## 25.1 Objective

Write a `Triangle` class and develop a Google Test suite that checks normal cases, boundary cases and invalid inputs.

## 25.2 The class

```cpp
class Triangle {
public:
    bool isValid(int a, int b, int c);
    std::string getType(int a, int b, int c);
};
```

The program must:

* check whether three sides can form a valid triangle,
* classify a valid triangle as **Equilateral**, **Isosceles** or **Scalene**,
* return **Invalid** for any invalid side combination.

A triangle is valid when **all sides are greater than 0** and **the sum of any two sides is greater than the third side**.

## 25.3 Required test cases

| Test Case | Input (a,b,c) | Expected Result |
|---|---|---|
| Equilateral | (5,5,5) | Equilateral |
| Isosceles | (5,5,3) | Isosceles |
| Scalene | (4,5,6) | Scalene |
| Invalid triangle | (1,2,3) | Invalid |
| Invalid triangle | (1,1,5) | Invalid |
| Equal boundary case | (2,2,4) | Invalid |
| Different order | (3,5,4) | Scalene |
| Zero side | (0,4,4) | Invalid |
| Negative side | (-2,4,5) | Invalid |

## 25.4 Suggested GTest structure

```cpp
TEST(TriangleTest, EquilateralTriangle) {
    Triangle t;
    EXPECT_TRUE(t.isValid(5, 5, 5));
    EXPECT_EQ(t.getType(5, 5, 5), "Equilateral");
}

TEST(TriangleTest, InvalidTriangle) {
    Triangle t;
    EXPECT_FALSE(t.isValid(1, 2, 3));
}
```

## 25.5 Bug-finding activity

After your tests pass, introduce **one** bug on purpose. For example, change

```cpp
if (a + b > c)      // correct
```

to

```cpp
if (a + b >= c)     // BUG: allows degenerate triangles such as (1,2,3)
```

Then:

1. Run the test suite.
2. Identify the failed tests.
3. Compare expected and actual results.
4. Locate the defective statement.
5. Correct the code.
6. Run the tests again.
7. Show that all tests pass.

**Which tests detect the `>=` bug?** `InvalidTriangle` (1,2,3) and `EqualBoundaryCase` (2,2,4). The other tests pass, which shows why the boundary case (sum **equal** to third side) is essential.

## 25.6 Deliverables

| Deliverable | Requirement |
|---|---|
| Triangle source code | `triangle.h` and `triangle.cpp` |
| GTest file | At least 8 test cases |
| Test coverage | Normal, boundary and invalid inputs |
| Bug demonstration | Introduce and identify one defect |
| Final output | Screenshot showing all tests passed |
| Short analysis | Explain which test detected the introduced bug |

## 25.7 Build and run

```bash
cd TriangleProject_Student
g++ triangle.cpp triangle_test.cpp -lgtest -pthread -o triangle_test
./triangle_test
```

## 25.8 Instructor reference solution

### `triangle.h`

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

### `triangle.cpp`

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

### `triangle_test.cpp` (14 tests)

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

### Marking guide (suggested, 100 marks)

| Item | Marks |
|---|---|
| `triangle.h` / `triangle.cpp` compile and work | 25 |
| At least 8 tests, all 9 table cases covered | 25 |
| Boundary tests ((2,2,4), (1,2,3)) | 15 |
| Invalid-input tests (zero, negative) | 10 |
| Bug demonstration (changed line, failed test, fix) | 15 |
| Screenshot + short analysis | 10 |

---

*End of laboratory handout.*
