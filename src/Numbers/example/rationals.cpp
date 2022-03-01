#include "Rational.hpp"
#include <iostream>

#define CHECK(condition) ((condition) ? "true" : "false")

int main() {
    Rational a(1, 2);
    Rational b(1, 3);
    Rational c(2, 4);
    Rational d(-1, 4);

    std::cout << a << " > " << b << " is " << CHECK(a > b) << std::endl;
    std::cout << a << " == " << c << " is " << CHECK(a == b) << std::endl;
    
    std::cout << "(" << a << ")^{-1}" << " = " << a.inv() << std::endl;
    std::cout << a << " + " << b << " = " << a + b << std::endl;
    std::cout << c << " - " << d << " = " << c - d << std::endl;
    std::cout << a << " * " << b << " = " << a * b << std::endl;
    std::cout << b << " + " << 0 << " = " << b + 0 << std::endl;
    std::cout << d << " / " << 2 << " = " << d / 2 << std::endl;
    
    try {
        a = d / 0;
    } catch(const std::domain_error& err) {
        std::cout << d << " / " << 0 << " leads to:\n";
        std::cout << err.what() << std::endl;
    }

    return 0;
}