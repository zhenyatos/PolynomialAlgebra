#include "Rational.hpp"
#include <exception>

class RationalError {
public:
    static const RationalError DIVISION_BY_ZERO;

    constexpr operator const char*() {
        return message[code];
    };

private:
    static const char* message[1];
    
    size_t code;

    RationalError(int code) 
        : code(code)
    {};
};

Rational::Rational(Integer n, Integer m) 
    : n(n), m(m) 
{
    if (m == 0)
        throw std::domain_error(RationalError::DIVISION_BY_ZERO);
    simplify();
}

Rational::Rational(const Rational& other)
    : n(other.n), m(other.m)
{}

Rational& Rational::operator=(const Rational& other) {
    n = other.n;
    m = other.m;
    return *this;
}

void Rational::simplify() {
    if (n == 0) {
        m = 1;
        return;
    }
    Integer gcd = GCD(n, m);
    n = div(n, gcd);
    m = div(m, gcd);
}

const RationalError RationalError::DIVISION_BY_ZERO = RationalError(0);

const char* RationalError::message[1] = {
        "Division by zero",
    };