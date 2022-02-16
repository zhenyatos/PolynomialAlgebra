#include "Integer.hpp"
#include "Rational.hpp"
#include <iostream>
#include <exception>
#include <limits>

int main() {
    Integer a = 8;
    Integer b = 34;
    Integer res;
    while (b != 0) {
        Integer r = a.rem(b);
        a = b;
        b = r;
    }
    std::cout << a << " " << GCD(a, b);
    Rational q(3, 2);
    return 0;
}