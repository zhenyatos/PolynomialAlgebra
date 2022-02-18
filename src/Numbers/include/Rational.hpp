#pragma one
#include "Integer.hpp"
#include <iostream>

class Rational {
public:
    Rational(Integer n, Integer m = 1);
    Rational(const Rational& other);
    Rational& operator=(const Rational& other);
    ~Rational() = default;

    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational& operator/=(const Rational& other);
    Rational operator-() const; 
    Rational inv() const;

    bool operator==(const Rational& other) const;
    bool operator!=(const Rational& other) const;
    bool operator>=(const Rational& other) const;
    bool operator>(const Rational& other) const;
    bool operator<=(const Rational& other) const;
    bool operator<(const Rational& other) const;

    Rational abs() const;
    
    friend std::ostream& operator<<(std::ostream& stream, const Rational& rational);

private:
    Integer n;
    Integer m;

    void simplify();
};

Rational operator+(Rational a, const Rational& b);
Rational operator-(Rational a, const Rational& b);
Rational operator*(Rational a, const Rational& b);
Rational operator/(Rational a, const Rational& b);