#include "Modular.hpp"
#include <iostream>

int main() {
    Modular a(2, 5);
    Modular b(3, 5);

    try {
        Modular bad(0, -1);
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }

    Modular c = a + b;
    Modular d = a - b;
    Modular e = b * b;
    std::cout << c << " " << d << " " << e << std::endl;

    Modular t(5, 13);
    std::cout << t.inv() << std::endl;

    try {
        Modular(2, 4).inv();
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }

    return 0;
}