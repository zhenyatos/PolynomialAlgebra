#include "Integer.hpp"
#include <iostream>

#define CHECK(condition) ((condition) ? "true" : "false")

int main() {
    Integer a = 3;
    Integer b = -4;
    Integer big = INT_MAX;
    Integer small = INT_MIN;

    std::cout << big << " > " << small << " is " << CHECK(big > small) << std::endl;
    std::cout << a << " >= " << b << " is " << CHECK(a >= b) << std::endl;
    std::cout << a << " == " << a << " is " << CHECK(a == a) << std::endl;

    std::cout << a << " + " << b << " = " << a + b << std::endl;
    std::cout << a << " - " << b << " = " << a - b << std::endl;
    std::cout << a << " * " << b << " = " << a * b << std::endl;

    try {
        a = big + 1;
    } catch(const std::overflow_error& err) {
        std::cout << big << " + " << 1 << " leads to:\n";
        std::cout << err.what() << std::endl;
    }

    try {
        b = small - 1;
    } catch(const std::overflow_error& err) {
        std::cout << small << " - " << 1 << " leads to:\n";
        std::cout << err.what() << std::endl;
    }

    Integer k;
    try {
        Integer x = 1;
        for (k = 1; k <= 100; k += 1)
            x *= 10;
    } catch(const std::overflow_error& err) {
        std::cout << "After raising 10 to the power of " << k << ":\n";
        std::cout << err.what() << std::endl;
    } 
}