#pragma once
#include "Integer.hpp"
#include <stdexcept>

class Modular {
public:
    Modular();
    Modular(Integer a, Integer N);
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
        stream << "[" << modular.val << ", " << modular.N << "]";
        return stream;
    }

private:
    Integer val;
    Integer N;

    void checkModulus();
};

Modular operator+(Modular a, const Modular& b);
Modular operator-(Modular a, const Modular& b);
Modular operator*(Modular a, const Modular& b);
Modular operator/(Modular a, const Modular& b);