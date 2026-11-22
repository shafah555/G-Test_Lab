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
