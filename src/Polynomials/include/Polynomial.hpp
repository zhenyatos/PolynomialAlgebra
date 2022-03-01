#pragma once
#include <algorithm>
#include <exception>
#include <limits>
#include <vector>

template<class T>
class Polynomial {
public:
    Polynomial();
    Polynomial(std::vector<T> coeff);
    Polynomial(const Polynomial& other);
    Polynomial& operator=(const Polynomial& other);
    ~Polynomial() = default;

    Polynomial& operator+=(const Polynomial& other);
    Polynomial& operator-=(const Polynomial& other);
    Polynomial& operator*=(const Polynomial& other);
    Polynomial& operator*=(const T& a);
    Polynomial operator-() const; 
    
    T operator()(T x) const;
    int deg() const;

    std::pair<Polynomial, Polynomial> divRem(const Polynomial& other) const;
    Polynomial div(const Polynomial& other) const;
    Polynomial rem(const Polynomial& other) const;

    friend std::ostream& operator<<(std::ostream& stream, const Polynomial<T>& p) {
        if (!p.isZero()) {
            stream << p.coeff[0];
            for (size_t i = 1; i < p.coeff.size(); i++)
                stream << " + " << p.coeff[i] << ".x^" << i;
        }
        else
            stream << T(0);
        
        return stream;
    }
    
private:
    std::vector<T> coeff;

    void reduce();
    inline bool isZero() const { return (coeff.size() == 0); }
    inline T lead() const { return coeff.back(); }
};

template<class T>
Polynomial<T>::Polynomial() 
    : coeff()
{}

template<class T>
Polynomial<T>::Polynomial(std::vector<T> coeff)
    : coeff(coeff)
{
    reduce();    
}

template<class T>
Polynomial<T>::Polynomial(const Polynomial<T>& other) {
    coeff = other.coeff;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator=(const Polynomial<T>& other) {
    coeff = other.coeff;
    return *this;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator+=(const Polynomial<T>& other) {
    if (isZero())
        return *this = other;
    if (coeff.size() < other.coeff.size())
        coeff.resize(other.coeff.size(), T(0));
    int size = std::min(coeff.size(), other.coeff.size());
    std::transform(coeff.begin(), coeff.begin() + size, other.coeff.begin(), coeff.begin(), std::plus<T>());
    reduce();
    return *this;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial<T>& other) {
    if (isZero())
        return *this = -other;
    if (coeff.size() < other.coeff.size())
        coeff.resize(other.coeff.size(), T(0)); 
    int size = std::min(coeff.size(), other.coeff.size());
    std::transform(coeff.begin(), coeff.begin() + size, other.coeff.begin(), coeff.begin(), std::minus<T>());
    reduce();
    return *this;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator*=(const Polynomial<T>& other) {
    if (isZero())
        return *this;
    if (other.isZero()) {
        *this = other;
        return *this;
    }
    std::vector<T> resCoeff(deg() + other.deg() + 1);
    for (int k = 0; k < resCoeff.size(); k++) {
        for (int i = 0; i <= k; i++) {
            if (i > deg() || k - i > other.deg())
                continue;
            resCoeff[k] += coeff[i] * other.coeff[k - i];
        }
    }
    coeff = resCoeff;
    return *this;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator*=(const T& a) {
    if (isZero())
        return *this;
    if (a == T(0)) {
        *this = Polynomial();
        return *this;
    }
    for (int i = 0; i < coeff.size(); i++)
        coeff[i] *= a;
    return *this;
}

template<class T>
Polynomial<T> Polynomial<T>::operator-() const {
    if (isZero())
        return Polynomial();
    Polynomial res = *this;
    for(int i = 0; i < coeff.size(); i++)
        res.coeff[i] = -res.coeff[i];
    return res;
}

template<class T>
T Polynomial<T>::operator()(T x) const {
    if (isZero())
        return T(0);
    if (x == T(0)) 
        return coeff[0];
    int i = coeff.size() - 1;
    T res = coeff[i];
    for(i--; i >= 0; i--)
        (res *= x) += coeff[i]; 
    return res;
}

template<class T>
int Polynomial<T>::deg() const {
    if (isZero())
        return INT_MIN;
    return coeff.size() - 1;
}

template<class T>
std::pair<Polynomial<T>, Polynomial<T>> Polynomial<T>::divRem(const Polynomial<T>& other) const {
    if (other.isZero())
        throw std::domain_error("Division by zero");
    
    if (other.deg() == 0)
        return {*this * other.coeff[0].inv(), Polynomial()};

    Polynomial q;
    Polynomial r = *this;

    while (r.deg() >= other.deg()) {
        int deg = r.deg() - other.deg();
        std::vector<T> xCoeff(deg+1, T(0));
        xCoeff[deg] = r.lead() / other.lead();
        Polynomial t = Polynomial(xCoeff);
        
        q += t;
        r -= t * other;
    }

    return {q, r};
}

template<class T>
Polynomial<T> Polynomial<T>::div(const Polynomial<T>& other) const {
    return divRem(other).first;
}

template<class T>
Polynomial<T> Polynomial<T>::rem(const Polynomial<T>& other) const {
    return divRem(other).second;
}

template<class T>
void Polynomial<T>::reduce() {
    int index = coeff.size() - 1;
    for (; index >= 0; index--)
        if (coeff[index] != T(0))
            break;
    if (coeff[index] != T(0))
        coeff.resize(index+1);
    else
        coeff.clear();
}

template<class T>
Polynomial<T> operator+(Polynomial<T> a, const Polynomial<T>& b) {
    a += b;
    return a;
}

template<class T>
Polynomial<T> operator-(Polynomial<T> a, const Polynomial<T>& b) {
    a -= b;
    return a;
}

template<class T>
Polynomial<T> operator*(Polynomial<T> a, const Polynomial<T>& b) { 
    a *= b;
    return a;
}

template<class T>
Polynomial<T> operator*(Polynomial<T> p, const T& a) {
    p *= a;
    return p;
}

template<class T>
inline Polynomial<T> norm(const Polynomial<T>& a) { return a.deg(); }

template<class T>
inline Polynomial<T> div(const Polynomial<T> a, const Polynomial<T>& b) { return a.div(b); }

template<class T>
inline Polynomial<T> rem(const Polynomial<T>& a, const Polynomial<T>& b) { return a.rem(b); }