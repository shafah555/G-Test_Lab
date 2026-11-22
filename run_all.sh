#!/usr/bin/env bash
cd "$(dirname "$0")" || exit 1
for p in "CalculatorProject calculator" "StudentGradeProject student_grade" "TriangleProject triangle"; do
  set -- $p; echo "=== $1 ==="
  ( cd $1 && g++ -std=c++17 $2.cpp ${2}_test.cpp -lgtest -pthread -o /tmp/lab_bin && /tmp/lab_bin | tail -3 )
done; rm -f /tmp/lab_bin
