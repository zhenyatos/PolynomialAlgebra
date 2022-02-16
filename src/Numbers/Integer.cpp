#include "Integer.hpp"
#include <exception>

class IntegerError {
public:
    static const IntegerError ADDITION_OVERFLOW;
    static const IntegerError SUBSTRACTION_OVERFLOW;
    static const IntegerError TAKING_OPPOSITE_OVERFLOW;
    static const IntegerError MULTIPLICATION_OVERFLOW;
    static const IntegerError DIVISION_BY_ZERO;

    constexpr operator const char*() {
        return message[code];
    };

private:
    static const char* message[5];
    
    size_t code;

    IntegerError(int code) 
        : code(code)
    {};
};

Integer::Integer(int val) 
    : val(val)
{}

Integer::Integer(const Integer& other) 
    : val(other.val)
{}

Integer& Integer::operator=(const Integer& other) {
    val = other.val;
    return *this;
}

Integer::operator int() const {
    return val;
}

Integer& Integer::operator+=(const Integer& other) {
    if (((val >= 0) && (other.val > INT_MAX - val)) || 
        ((val < 0) && (other.val < INT_MIN - val)))
            throw std::overflow_error(IntegerError::ADDITION_OVERFLOW);
    val += other.val;
    return *this;
}

Integer& Integer::operator-=(const Integer& other) {
    if (((other.val >= 0) && (val < INT_MIN + other.val)) ||
        ((other.val < 0) && (val > INT_MAX + other.val)))
            throw std::overflow_error(IntegerError::SUBSTRACTION_OVERFLOW);
    val -= other.val;
    return *this;
}

Integer& Integer::operator*=(const Integer& other) {
    if (((val > 0) && (other.val > 0) && (val > INT_MAX / other.val)) ||
        ((val > 0) && (other.val < 0) && (other.val < INT_MIN / val)) ||
        ((val < 0) && (other.val > 0) && (val < INT_MIN / other.val)) ||
        ((val < 0) && (other.val < 0) && (other.val < INT_MAX / val)))
            throw std::overflow_error(IntegerError::MULTIPLICATION_OVERFLOW);
    val *= other.val;
    return *this;

}

Integer Integer::operator-() const {
    if (val == INT_MIN)
        throw std::overflow_error(IntegerError::MULTIPLICATION_OVERFLOW);
    return -val;
}

Integer operator+(Integer a, const Integer& b) {
    a += b;
    return a;
}

Integer operator-(Integer a, const Integer& b) {
    a -= b;
    return a;
}

Integer operator*(Integer a, const Integer& b) {
    a *= b;
    return a;
}

bool Integer::operator==(const Integer& other) const {
    return val == other.val;
}

bool Integer::operator!=(const Integer& other) const {
    return val != other.val;
}

bool Integer::operator>=(const Integer& other) const {
    return val >= other.val;
}

bool Integer::operator>(const Integer& other) const {
    return val > other.val;
}

bool Integer::operator<(const Integer& other) const {
    return val < other.val;
}

bool Integer::operator<=(const Integer& other) const {
    return val <= other.val;
}

Integer Integer::abs() const {
    if (val < 0) {
        if (val == INT_MIN)
            throw std::overflow_error(IntegerError::TAKING_OPPOSITE_OVERFLOW);
        else
            return -val;
    }
    return val;
}

Integer Integer::div(const Integer& other) const {
    if (other.val == 0)
        throw std::domain_error(IntegerError::DIVISION_BY_ZERO);
    return val / other.val;
}

Integer Integer::rem(const Integer& other) const {
    if (other.val == 0) 
        throw std::domain_error(IntegerError::DIVISION_BY_ZERO);
    return val % other.val;
}

Integer GCD(const Integer& a, const Integer& b) {
    int x = int(a);
    int y = int(b);
    while (y != 0) {
        int r = x % y;
        x = y;
        y = r;
    }
    return x;
}

std::ostream& operator<<(std::ostream& stream, const Integer& integer) {
    stream << integer.val;
    return stream;
}

const IntegerError IntegerError::ADDITION_OVERFLOW = IntegerError(0);
const IntegerError IntegerError::SUBSTRACTION_OVERFLOW = IntegerError(1);
const IntegerError IntegerError::TAKING_OPPOSITE_OVERFLOW = IntegerError(2);
const IntegerError IntegerError::MULTIPLICATION_OVERFLOW = IntegerError(3);
const IntegerError IntegerError::DIVISION_BY_ZERO = IntegerError(4);

const char* IntegerError::message[5] = {
        "Integer addition overflow",
        "Integer substraction overflow",
        "Taking opposite integer overflow",
        "Integer multiplication overflow",
        "Division by zero",
    };