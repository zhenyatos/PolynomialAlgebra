#pragma one
#include "Integer.hpp"
#include <iostream>

class Rational {
public:
    Rational(Integer n, Integer m = 1);
    Rational(const Rational& other);
    Rational& operator=(const Rational& other);
    ~Rational() = default;

private:
    Integer n;
    Integer m;

    void simplify();
};