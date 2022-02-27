#pragma once
#include <vector>
#include <limits>
#include <algorithm>

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
    Polynomial operator-() const; 
    
    T operator()(T x) const;
    int deg() const;

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
    std::transform(coeff.begin(), coeff.end(), other.coeff.begin(), coeff.begin(), std::plus<T>());
    reduce();
    return *this;
}

template<class T>
Polynomial<T>& Polynomial<T>::operator-=(const Polynomial<T>& other) {
    if (isZero())
        return *this = other;
    if (coeff.size() < other.coeff.size())
        coeff.resize(other.coeff.size(), T(0));
    std::transform(coeff.begin(), coeff.end(), other.coeff.begin(), coeff.begin(), std::minus<T>());
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
void Polynomial<T>::reduce() {
    int index = coeff.size() - 1;
    for (; index >= 0; index--)
        if (coeff[index] != 0)
            break;
    coeff.resize(index+1);
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