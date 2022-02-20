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

    int deg() const;

    friend std::ostream& operator<<(std::ostream& stream, const Polynomial<T>& p) {
        if (p.coeff.size() != 0) {
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
    if (coeff.size() == 0)
        return *this = other;
    if (coeff.size() < other.coeff.size())
        coeff.resize(other.coeff.size(), T(0));
    std::transform(coeff.begin(), coeff.end(), other.coeff.begin(), coeff.begin(), std::plus<T>());
    reduce();
    return *this;
}

template<class T>
int Polynomial<T>::deg() const {
    if (coeff.size() == 0)
        return INT_MIN;
    return coeff.size() - 1;
}

template<class T>
void Polynomial<T>::reduce() {
    size_t index = coeff.size() - 1;
    for (; index >= 0; index--)
        if (coeff[index] != 0)
            break;
    coeff.resize(index+1);
}
