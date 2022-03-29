# PolynomialAlgebra
> Console calculator with support of the Integers, Rationals, Modular arithmetic, as well as the Polynomials over these types

## Requirements
- GCC 7.3.0 (C++ 11)
- CMake

## Build
To compile the project, run the following command in the root folder:

`cmake --build build --config Release --target all -j`

This should generate **palgebra** executable inside `build` folder. Now you can run it and move to the next steps.

## Usage
Usage is fairly intuitive, so I have decided to include some examples and that's it.

### Basic arithmetic

![image](https://user-images.githubusercontent.com/47058532/160654374-cc0644d6-a5c5-415f-85f3-b837336fb299.png)

Notes:
- `p//q` stands for the rational number with numerator `p` and denominator `q`
- `[a, N]` stands for `a` modulo `N`
- Implicit type conversion is implemented between `INTEGER` and `RATIONAL` data types

### Variables

![image](https://user-images.githubusercontent.com/47058532/160656700-e24b4cd5-3368-48f4-8c3b-5f8c3c0fb52b.png)

Notes:
- Variable type is deduced from the expression on the right side of the assignment operator
- `;` serves as the separator in complex sentences
- `print` command is introduced here. It may seem unnecessary, yet it's possible to use it in complex sentences
