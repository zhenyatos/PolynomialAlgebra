#pragma once
#include <iostream>

class Integer {
public:
    Integer(int val = 0);
    Integer(const Integer& other);
    Integer& operator= (const Integer& other);
    ~Integer() = default;
    explicit operator int() const;

    Integer& operator+=(const Integer& other);
    Integer& operator-=(const Integer& other);
    Integer& operator*=(const Integer& other);
    Integer operator-() const; 

    bool operator==(const Integer& other) const;
    bool operator!=(const Integer& other) const;
    bool operator>=(const Integer& other) const;
    bool operator>(const Integer& other) const;
    bool operator<(const Integer& other) const;
    bool operator<=(const Integer& other) const;

    Integer abs() const;

    friend std::ostream& operator<<(std::ostream& stream, const Integer& integer);

private:
    int val;
};

Integer operator+(Integer a, const Integer& b);
Integer operator-(Integer a, const Integer& b);
Integer operator*(Integer a, const Integer& b);
inline Integer norm(const Integer& other) { return other.abs(); }