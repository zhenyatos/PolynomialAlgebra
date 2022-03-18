#include "Rational.hpp"
#include <stdexcept>

class RationalError {
public:
    static const RationalError DIVISION_BY_ZERO;
    static const RationalError NO_INVERSE;

    constexpr operator const char*() {
        return message[code];
    };

private:
    static const char* message[2];
    
    size_t code;

    RationalError(size_t code) 
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

Rational::Rational(int n)
    : n(n), m(1)
{
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

Rational& Rational::operator+=(const Rational& other) {
    n *= other.m;
    n += other.n * m;
    m *= other.m;
    simplify();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    (*this) += -other;
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    n *= other.n;
    m *= other.m;
    simplify();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    (*this) *= other.inv(); 
    return *this;
}

Rational Rational::operator-() const {
    return Rational(-n, m);
}

Rational Rational::inv() const {
    if (n == 0)
        throw std::domain_error(RationalError::NO_INVERSE);
    return Rational(m, n);
}

bool Rational::operator==(const Rational& other) const {
    return (n == other.n) && (m == other.m);
}

bool Rational::operator!=(const Rational& other) const {
    return (n != other.n) || (m != other.m);
}

bool Rational::operator>=(const Rational& other) const {
    return n * other.m >= other.n * m;
}

bool Rational::operator>(const Rational& other) const {
    return n * other.m > other.n * m;
}

bool Rational::operator<=(const Rational& other) const {
    return n * other.m <= other.n * m;
}

bool Rational::operator<(const Rational& other) const {
    return n * other.m < other.n * m;
}

void Rational::simplify() {
    if (n == 0) {
        m = 1;
        return;
    }

    Integer gcd = GCD(n, m);
    n = div(n, gcd);
    m = div(m, gcd);

    if (m < 0) {
        m = -m;
        n = -n;
    }
}

Rational operator+(Rational a, const Rational& b) {
    a += b;
    return a;
}

Rational operator-(Rational a, const Rational& b) {
    a -= b;
    return a;
}

Rational operator*(Rational a, const Rational& b) {
    a *= b;
    return a;
}

Rational operator/(Rational a, const Rational& b) {
    a /= b;
    return a;
}

Rational Rational::abs() const {
    return Rational(n.abs(), m);
}
    
std::ostream& operator<<(std::ostream& stream, const Rational& rational) {
    stream << rational.n << "//" << rational.m;
}

const RationalError RationalError::DIVISION_BY_ZERO = RationalError(0);
const RationalError RationalError::NO_INVERSE = RationalError(1);

const char* RationalError::message[2] = {
        "Division by zero",
        "Zero has no multiplicative inverse",
    };