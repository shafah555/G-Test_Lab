#include "triangle.h"

bool Triangle::isValid(int a, int b, int c) {
    // Every side must be positive (rejects zero and negative sides)
    if (a <= 0 || b <= 0 || c <= 0) {
        return false;
    }

    // Triangle inequality: the sum of any two sides must be GREATER than the third.
    // long long avoids integer overflow for very large sides (e.g. INT_MAX).
    long long x = a, y = b, z = c;
    return (x + y >= z) && (x + z >= y) && (y + z >= x);
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
