#include "Modular.hpp"

class ModularError {
public:
    static const ModularError NEGATIVE_MODULUS;
    static const ModularError NO_INVERSE;
    static const ModularError DIFF_MODULUS;

    constexpr operator const char*() {
        return message[code];
    }

private:
    static const char* message[3];

    size_t code;

    ModularError(size_t code)
        : code(code)
    {}
};


Modular::Modular() 
    : N(0), val(0)
{}

Modular::Modular(Integer a, Integer N = 0) 
    : N(N)
{
    if (N < 0)
        throw std::domain_error(ModularError::NEGATIVE_MODULUS);
    if (N == 0)
        val = 0;
    else
        val = (a.rem(N) + N).rem(N);
}

Modular::Modular(const Modular& other) {
    N = other.N;
    val = other.val;
}

Modular& Modular::operator=(const Modular& other) {
    if (N == 0) {
        N = other.N;
        val = other.val;
    } else if (N == other.N) 
        val = other.val;
    else
        throw std::invalid_argument(ModularError::DIFF_MODULUS);
    return *this;
}

Modular& Modular::operator+=(const Modular& other) {
    if (N == 0) {
        N = other.N;
        val = other.val;
    } else if (N == other.N) {
        val += other.val;
        val = val.rem(N);
    } else if (other.N != 0)
        throw std::invalid_argument(ModularError::DIFF_MODULUS);
    return *this;
}

Modular& Modular::operator-=(const Modular& other) {
    if (N == 0) {
        N = other.N;
        val = (-other).val;
    } else if (N == other.N) {
        (*this) += (-other);
    } else if (other.N != 0)
        throw std::invalid_argument(ModularError::DIFF_MODULUS);
    return *this;
}

Modular& Modular::operator*=(const Modular& other) {
    if (N == 0) {
        N = other.N;
        val = 0;
    } else if (N == other.N) {
        val *= other.val;
        val = val.rem(N);
    } else if (other.N == 0)
        val = 0;
    else
        throw std::invalid_argument(ModularError::DIFF_MODULUS);
    return *this;
}

Modular& Modular::operator/=(const Modular& other) {
    if (N == 0) {
        N = other.N;
        val = other.val;
    } else if (N == other.N)
        (*this) *= other.inv();
    else if (other.N == 0)
        throw std::invalid_argument(ModularError::NO_INVERSE);
    else
        throw std::invalid_argument(ModularError::DIFF_MODULUS);
    return *this;
}

Modular Modular::operator-() const {
    if (N == 0)
        return Modular();
    return Modular(N - val, N);
}

Modular Modular::inv() const {
    if (val == 0)
        throw std::domain_error(ModularError::NO_INVERSE);
    if (val == 1)
        return Modular(1, N);
    Integer x = N;
    Integer y = val;
    Integer b2, b1 = 1, b0 = 0;
    Integer r, q;

    do {
        r = x.rem(y);
        q = x.div(y);
        x = y;
        y = r;
        b2 = b0 - q * b1;
        b0 = b1;
        b1 = b2;
    } while (r != 0 && r != 1);

    if (r == 0)
        throw std::domain_error(ModularError::NO_INVERSE); 

    return Modular(b2, N);
}

bool Modular::operator==(const Modular& other) {
    return val == other.val;
}

bool Modular::operator!=(const Modular& other) {
    return val != other.val;
}

Modular operator+(Modular a, const Modular& b) {
    a += b;
    return a;
}

Modular operator-(Modular a, const Modular& b) {
    a -= b;
    return a;
}

Modular operator*(Modular a, const Modular& b) {
    a *= b;
    return a;
}

Modular operator/(Modular a, const Modular& b) {
    a /= b;
    return a;
}

const ModularError ModularError::NEGATIVE_MODULUS = ModularError(0);
const ModularError ModularError::NO_INVERSE = ModularError(1);
const ModularError ModularError::DIFF_MODULUS = ModularError(2);

const char* ModularError::message[3] = {
        "Modulus must be a positive integer",
        "The number has no multiplicative inverse by this modulus",
        "Operands have different modulus"
    };