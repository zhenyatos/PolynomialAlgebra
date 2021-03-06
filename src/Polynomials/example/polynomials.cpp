#include "Integer.hpp"
#include "Rational.hpp"
#include "Polynomial.hpp"

int main() { 
    Polynomial<Integer> p({1, 0, -2});
    std::cout << "Polynomial p = " << p << " of degree " << p.deg() << std::endl;
    Polynomial<Integer> q({1, 2, 2, 0, 0});
    std::cout << "Polynomial q = " << q << " of degree " << q.deg() << std::endl;
    
    std::cout << "\nOne can evaluate the polynomial at some point:\n";
    Integer x = 5;
    std::cout << "p(" << x << ") = " << p(x) << std::endl; 
    
    std::cout << "\nOne can calculate the sum of two polynomials:\n";
    std::cout << "p + q = " << p + q << std::endl;
    std::cout << "\nAlso, one can calculate the difference of two polynomials:\n";
    std::cout << "p - q = " << p - q << std::endl;

    std::cout << "\nOne can multiply two polynomials:";
    auto t = p * q;
    std::cout << "\nt = p * q = " << p * q;
    std::cout << "\nWe have obtained the polynomial of degree " << t.deg() << std::endl;

    std::cout << "\nAlso, it's possible to multiply a polynomial by a scalar:";
    p *= 2;
    std::cout << "\np * 2 = " << p << std::endl;

    std::cout << "\nThe degree of the zero polynomial is " << Polynomial<Integer>().deg();
    std::cout << "\nIt is smaller than any other possible degree except itself, but that's ok, I guess" << std::endl;

    std::cout << "\nIt is possible to use any data type with ring-like structure in terms of operations.\n";
    std::cout << "\nFor example, the Rationals can be used:";
    std::vector<Rational> coeff = {Rational(1, 2), Rational(-1, 3)};
    Polynomial<Rational> v(coeff);
    std::cout << "\nPolynomial v = " << v << " of degree " << v.deg() << std::endl;
    std::cout << "v * v = " << v * v << std::endl;

    std::cout << "\nIt is possible to divide polynomials with remainder ";
    std::cout << "but only for the data types supporting division operation:";
    Polynomial<Rational> a({-2, 1, 2, 1});
    Polynomial<Rational> b({-2, 1});
    std::cout << "\na = " << a << "\nb = " << b;
    std::cout << "\na / b gives us:";
    auto result = a.divRem(b);
    Polynomial<Rational> c = result.first;
    Polynomial<Rational> r = result.second;
    std::cout << "\nc = " << c << "\nr = " << r;
    std::cout << "\ndeg(r) = " << r.deg() << " and deg(r) < deg(b) = " << b.deg();
    std::cout << "\nNow, as you can see: a = c * b + r = " << c * b + r;
    std::cout << "\nAlso, note that a(2) = " << a(2) << " = r" << std::endl;

    return 0;
}
