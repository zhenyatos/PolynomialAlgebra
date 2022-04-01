# PolynomialAlgebra
> Console calculator with support of the Integer, Rational, Modular arithmetic, as well as the Polynomials over these types

## Requirements
- GCC 7.3.0 (C++ 11)
- CMake

## Build
To compile the project, run the following command in the root folder:

`cmake --build build --config Release --target all -j`

This should generate **palgebra** executable inside `build` folder. Now you can run it and move to the next steps. Note that you can use `exit()` command to close the application.

## Usage
Usage is fairly intuitive, so I have decided to include some examples and that's it.

### Basic arithmetic

![image](https://user-images.githubusercontent.com/47058532/160654374-cc0644d6-a5c5-415f-85f3-b837336fb299.png)

Notes:
- `p//q` stands for the rational number with numerator `p` and denominator `q`
- `[a, N]` stands for `a` modulo `N`
- Implicit type conversion between `INTEGER` and `RATIONAL` data types is implemented

### Variables

![image](https://user-images.githubusercontent.com/47058532/160656700-e24b4cd5-3368-48f4-8c3b-5f8c3c0fb52b.png)

Notes:
- Variable type is deduced from the expression on the right side of the assignment operator
- `;` serves as the separator in complex sentences
- `print` command is introduced here. It may seem unnecessary, yet it's possible to use it in complex sentences

### Polynomial arithmetic

![image](https://user-images.githubusercontent.com/47058532/161214680-13fdd978-eeb3-4eef-b61f-5fefc05626e6.png)

Notes:
- Monomial is represented as `c.X^n` where `c` - coefficient (`INTEGER`, `RATIONAL` or `MODULAR`) and `X^n` means that monomial is of degree `n`
- Implicit type conversion between `POLYNOMIAL{INTEGER}` and `POLYNOMIAL{RATIONAL}` data types is implemented 
- `eval` command is introduced here: for polynomial `poly` of type `POLYNOMIAL{T}`, `eval(poly, x)`, where `x` is of type `T` means evaluation in point `x`. In other words, this command allows us to treat polynomials as functions

### GCD calculation

![image](https://user-images.githubusercontent.com/47058532/161216918-be4b5f7e-8563-417e-a51e-397c2a578b57.png)

Notes: 
- `gcd` method is implemented for the following data types: `INTEGER`, `POLYNOMIAL{RATIONAL}` and `POLYNOMIAL{MODULAR}`, although for the non-prime modulus it cannot be guaranteed that the method will return the correct result
