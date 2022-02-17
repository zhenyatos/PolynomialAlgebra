#include "Integer.hpp"
#include "Rational.hpp"
#include <iostream>
#include <exception>
#include <limits>

int main() {
    Rational a(3, 2);
    Rational b(6, 4);
    Rational c(-1, -2);
    Rational d(1, 2);
    std::cout << (a == b) << " " << (a > c) << " " << (c != d);
    return 0;
}