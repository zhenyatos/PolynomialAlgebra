#include "Modular.hpp"
#include <iostream>

int main() {
    Modular<5> a(2);
    Modular<5> b(3);

    try {
        Modular<-1> bad;
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }

    Modular<5> c = a + b;
    Modular<5> d = a - b;
    Modular<5> e = b * b;
    std::cout << c << " " << d << " " << e << std::endl;

    Modular<4> t(3);
    std::cout << t.inv() * t << std::endl;

    try {
        Modular<4>(2).inv();
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }

    return 0;
}