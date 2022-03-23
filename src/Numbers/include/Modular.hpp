#pragma once
#include "Integer.hpp"
#include <stdexcept>

class Modular {
public:
    Modular();
    Modular(int a);
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

    bool operator==(const Modular& other) const;
    bool operator!=(const Modular& other) const;

    friend std::ostream& operator<<(std::ostream& stream, const Modular& modular) {
        if (modular == Modular(0))
            stream << 0;
        else
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