add_executable(e-polynomials polynomials.cpp)

target_link_libraries(e-polynomials numbers polynomials)

set_target_properties(e-polynomials PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\..\\bin
        )