#!/usr/bin/env bash
# Builds and runs every project in this lab. Needs g++ and libgtest-dev.
cd "$(dirname "$0")" || exit 1
build_run () {
  dir=$1; shift
  echo "=============== $dir ==============="
  ( cd "$dir" && g++ -std=c++17 "$@" -lgtest -pthread -o /tmp/gtest_lab_bin && /tmp/gtest_lab_bin | tail -4 )
}
build_run CalculatorProject calculator.cpp calculator_test.cpp
build_run StudentGradeProject_Instructor student_grade.cpp student_grade_test.cpp
build_run TriangleProject_Instructor triangle.cpp triangle_test.cpp
echo
echo "Starter projects are SUPPOSED to fail until students implement them:"
build_run StudentGradeProject_Student student_grade.cpp student_grade_test.cpp
build_run TriangleProject_Student triangle.cpp triangle_test.cpp
rm -f /tmp/gtest_lab_bin
