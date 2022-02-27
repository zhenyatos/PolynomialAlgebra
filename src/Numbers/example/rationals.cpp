#include "Rational.hpp"
#include <iostream>

#define CHECK(condition) ((condition) ? "true" : "false")

int main() {
    Rational a(1, 2);
    Rational b(1, 3);
    Rational c(2, 4);
    Rational d(-1, 4);
    Rational zero(0, 1);

    std::cout << a << " > " << b << " is " << CHECK(a > b) << std::endl;
    std::cout << a << " == " << c << " is " << CHECK(a == b) << std::endl;
    
    std::cout << "(" << a << ")^{-1}" << " = " << a.inv() << std::endl;
    std::cout << a << " + " << b << " = " << a + b << std::endl;
    std::cout << c << " - " << d << " = " << c - d << std::endl;
    std::cout << a << " * " << b << " = " << a * b << std::endl;
    std::cout << b << " + " << zero << " = " << b + zero << std::endl;
    
    try {
        a = d / zero;
    } catch(const std::domain_error& err) {
        std::cout << d << " / " << zero << " leads to:\n";
        std::cout << err.what() << std::endl;
    }

    return 0;
}