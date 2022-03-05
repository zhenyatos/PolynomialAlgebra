#include "Integer.hpp"
#include "Rational.hpp"
#include "Modular.hpp"
#include "Polynomial.hpp"
#include <iostream>

template<int N>
Modular<N> M(Integer a) {
    return Modular<N>(a);
}

int main() {
    Polynomial<Modular<3>> a({M<3>(1), M<3>(2)});
    std::cout << a * a;
    std::cout << "\na([1, 3]) = " << a(M<3>(1));
    return 0;
}