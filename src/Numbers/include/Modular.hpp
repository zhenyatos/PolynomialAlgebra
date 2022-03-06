#pragma once
#include "Integer.hpp"
#include "ModularError.hpp"
#include <stdexcept>

template<int N>
class Modular {
public:
    Modular();
    explicit Modular(Integer a);
    Modular(const Modular& other);    
    Modular& operator=(const Modular& other);
    ~Modular() = default;

    Modular& operator+=(const Modular& other);
    Modular& operator-=(const Modular& other);
    Modular& operator*=(const Modular& other);
    Modular& operator/=(const Modular& other);
    Modular operator-() const;
    Modular inv() const;

    bool operator==(const Modular& other);
    bool operator!=(const Modular& other);

    friend std::ostream& operator<<(std::ostream& stream, const Modular& modular) {
        stream << "[" << modular.val << ", " << N << "]";
        return stream;
    }

private:
    Integer val;

    void checkModulus();
};

template<int N>
Modular<N>::Modular() {
    checkModulus();
    val = 0;
}

template<int N>
Modular<N>::Modular(Integer a) {
    checkModulus();
    val = (a.rem(N) + N).rem(N);
}

template<int N>
Modular<N>::Modular(const Modular<N>& other) {
    checkModulus();
    val = other.val;
}

template<int N>
Modular<N>& Modular<N>::operator=(const Modular<N>& other) {
    val = other.val;
    return *this;
}

template<int N>
Modular<N>& Modular<N>::operator+=(const Modular<N>& other) {
    val += other.val;
    val = val.rem(N);
    return *this;
}

template<int N>
Modular<N>& Modular<N>::operator-=(const Modular<N>& other) {
    val += (N - other.val);
    val = val.rem(N);
    return *this;
}

template<int N>
Modular<N>& Modular<N>::operator*=(const Modular<N>& other) {
    val *= other.val;
    val = val.rem(N);
    return *this;
}

template<int N>
Modular<N>& Modular<N>::operator/=(const Modular<N>& other) {
    val *= other.inv();
    return *this;
}

template<int N>
Modular<N> Modular<N>::operator-() const {
    return Modular(N - val);
}

template<int N>
Modular<N> Modular<N>::inv() const {
    if (val == 0)
        throw std::domain_error(ModularError::NO_INVERSE);
    if (val == 1)
        return Modular<N>(1);
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

    return Modular<N>(b2);
}

template<int N>
bool Modular<N>::operator==(const Modular<N>& other) {
    return val == other.val;
}

template<int N>
bool Modular<N>::operator!=(const Modular<N>& other) {
    return val != other.val;
}

template<int N>
void Modular<N>::checkModulus() {
    if (N <= 0)
        throw std::domain_error(ModularError::NONPOSITIVE_MODULUS);
}

template<int N>
Modular<N> operator+(Modular<N> a, const Modular<N>& b) {
    a += b;
    return a;
}

template<int N>
Modular<N> operator-(Modular<N> a, const Modular<N>& b) {
    a -= b;
    return a;
}

template<int N>
Modular<N> operator*(Modular<N> a, const Modular<N>& b) {
    a *= b;
    return a;
}

template<int N>
Modular<N> operator/(Modular<N> a, const Modular<N>& b) {
    a /= b;
    return a;
}