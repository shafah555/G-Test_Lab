# GTest Lab — How to Build and Run

Commands for the C++ Google Test lab. Use a Linux/Ubuntu terminal (WSL also works).

## Folder structure

```
GTest_Lab/
├── Lab_Handout.md                      full lab handout
├── CalculatorProject/                  teacher demonstration
│   ├── calculator.h
│   ├── calculator.cpp
│   └── calculator_test.cpp
├── StudentGradeProject_Student/        student task (starter code)
├── StudentGradeProject_Instructor/     instructor solution (keep private)
├── TriangleProject_Student/            homework (starter code)
│   ├── triangle.h
│   ├── triangle.cpp
│   └── triangle_test.cpp
├── TriangleProject_Instructor/         homework solution, 14 tests (keep private)
├── outputs/                            real captured pass/fail outputs
├── Lab_Report.md / Lab_Report.docx     complete lab report
└── run_all.sh                          builds and runs every project
```

## 1. Setup (once)

```bash
sudo apt update
sudo apt install g++ libgtest-dev
unzip GTest_Lab_Final.zip
cd GTest_Lab
```

If you get `cannot find -lgtest`, build the library once:

```bash
sudo apt install cmake
cd /usr/src/googletest
sudo cmake -S . -B build && sudo cmake --build build
sudo cp build/lib/*.a /usr/lib
```

## 2. Calculator demo (teacher)

```bash
cd CalculatorProject
g++ calculator.cpp calculator_test.cpp -lgtest -pthread -o calculator_test
./calculator_test
cd ..
```

Expected: `[  PASSED  ] 5 tests.`

## 3. Student task (students)

```bash
cd StudentGradeProject_Student
g++ student_grade.cpp student_grade_test.cpp -lgtest -pthread -o student_grade_test
./student_grade_test
cd ..
```

The starter tests **fail** until students implement the functions in `student_grade.cpp`. This is intended.

## 4. Instructor solution

```bash
cd StudentGradeProject_Instructor
g++ student_grade.cpp student_grade_test.cpp -lgtest -pthread -o student_grade_test
./student_grade_test
```

Expected: `[  PASSED  ] 8 tests.`

## 5. Triangle homework

```bash
cd TriangleProject_Student
g++ triangle.cpp triangle_test.cpp -lgtest -pthread -o triangle_test
./triangle_test
cd ..
```

The starter tests fail until `isValid()` and `getType()` are implemented. Instructor solution: `TriangleProject_Instructor` -> `[  PASSED  ] 14 tests.`

Bug demo: in `triangle.cpp` change the three `>` comparisons in `isValid()` to `>=`. Tests `InvalidTriangle`, `EqualBoundaryCase`, `InvalidAnyOrder` and `SmallestValidTriangles` fail. Change back and rerun.

## 6. Run everything at once

```bash
chmod +x run_all.sh
./run_all.sh
```

## Command explained

| Part | Meaning |
|---|---|
| `g++` | C++ compiler |
| `student_grade.cpp student_grade_test.cpp` | source files compiled together |
| `-lgtest` | link Google Test |
| `-pthread` | required by Google Test |
| `-o student_grade_test` | name of the output program |

The test files contain their own `main()`, so `-lgtest_main` is not needed.

## Useful options

```bash
./student_grade_test --gtest_list_tests                            # list all tests
./student_grade_test --gtest_filter=StudentGradeTest.InvalidMarks  # run one test
./student_grade_test --gtest_color=yes                             # colored output
```

## Failed-test demo (optional)

In the instructor folder, edit `student_grade.cpp` and change `/ 3.0` to `/ 2.0` in `average()`. Rebuild and run: tests fail with `Expected = 80.0, Actual = 120.0`. Change it back to `/ 3.0` and rerun to see them pass.

## Common problems

| Problem | Fix |
|---|---|
| `gtest/gtest.h: No such file` | `sudo apt install libgtest-dev` |
| `cannot find -lgtest` | build the library (see Setup) |
| `undefined reference to pthread_create` | add `-pthread` |
| `undefined reference to StudentGrade::total` | compile `student_grade.cpp` in the same command |
| `multiple definition of main` | remove `-lgtest_main` or your own `main()` |