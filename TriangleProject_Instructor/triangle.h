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
