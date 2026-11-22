#include "student_grade.h"
#include <stdexcept>

// TODO: implement the three functions.
// Rules: marks must be 0..100 (inclusive), otherwise throw std::invalid_argument.

int StudentGrade::total(int a, int b, int c) {
    // TODO: validate marks, then return a + b + c
    return 0;
}

double StudentGrade::average(int a, int b, int c) {
    // TODO: validate marks, then return the average (use 3.0, not 3)
    return 0.0;
}

char StudentGrade::grade(double average) {
    // TODO: throw if average is outside 0..100; A>=80, B>=70, C>=60, D>=50, else F
    return '?';
}
