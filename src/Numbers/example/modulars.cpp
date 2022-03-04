#include "Modular.hpp"
#include <iostream>

int main() {
    Modular<3> a(1);
    Modular<3> b(3);
    Modular<3> c = a + b;
    std::cout << c << std::endl;

    try {
        Modular<-1> bad;
    } catch (const std::exception& err) {
        std::cout << err.what();
    }
    return 0;
}