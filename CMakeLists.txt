cmake_minimum_required(VERSION 3.17)
project(PolynomialAlgebra)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SHARED_LIBRARY_PREFIX "pa-")

add_subdirectory(src)

add_subdirectory(app)
