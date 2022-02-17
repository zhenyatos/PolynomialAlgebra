#pragma one
#include "Integer.hpp"
#include <iostream>

class Rational {
public:
    Rational(Integer n, Integer m = 1);
    Rational(const Rational& other);
    Rational& operator=(const Rational& other);
    ~Rational() = default;

    bool operator==(const Rational& other) const;
    bool operator!=(const Rational& other) const;
    bool operator>=(const Rational& other) const;
    bool operator>(const Rational& other) const;
    bool operator<=(const Rational& other) const;
    bool operator<(const Rational& other) const;

private:
    Integer n;
    Integer m;

    void simplify();
};