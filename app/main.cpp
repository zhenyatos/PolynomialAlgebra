#include "Integer.hpp"
#include "Rational.hpp"
#include "Polynomial.hpp"
#include <iostream>

int main() {
    Polynomial<Rational> z;
    std::cout << z << " " << z.deg() << std::endl;
    Polynomial<Integer> p({1, 0, -2});
    std::cout << p.deg() << std::endl;
    Polynomial<Integer> q({1, 2, 3, 0, 0});
    std::cout << q.deg() << std::endl;
    auto t1 = p + q;
    std::cout << p << " +\n" << q << " =\n" << t1 << std::endl;
    std::cout << t1(1) << std::endl;
    auto t2 = p * q;
    std::cout << t2;
    return 0;
}