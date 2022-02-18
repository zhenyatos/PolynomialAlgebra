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
    std::cout << (a == b) << " " << (a > c) << " " << !(c != d) << std::endl;

    Rational sum(2, 1);
    Rational diff(1, 1);
    Rational mult(3, 4);
    Rational div(-3, -1);
    std::cout << (a + d == sum) << " " << (a - d == diff) << std::endl;
    std::cout << (a * d == mult) << " " << (a / d == div) << std::endl;

    Rational zero(0, 1);
    std::cout << zero << " " << zero + Integer(1) << std::endl;
    try { 
        zero.inv(); 
    } catch (std::exception& err) {
        std::cout << "Damn\n";
    }
    return 0;
}