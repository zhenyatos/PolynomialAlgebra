#pragma once
#include "Integer.hpp"
#include "ModularError.hpp"
#include <exception>

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
    Integer x = N;
    Integer y = val;
    Integer b2, b1, b0 = 1;

    while (y != 0) {
        Integer r = x.rem(y);
        Integer q = x.div(y);
        x = y;
        y = r;
        b2 = b1 - q * b0;
        b0 = b1;
        b1 = b2;
    }

    if (x != 1)
        throw std::domain_error(ModularError::NO_INVERSE); 

    return Modular<N>(b2);
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