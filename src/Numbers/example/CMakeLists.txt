add_executable(e-integers integers.cpp)

target_link_libraries(e-integers numbers)

set_target_properties(e-integers PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\..\\bin
        )

add_executable(e-rationals rationals.cpp)

target_link_libraries(e-rationals numbers)

set_target_properties(e-rationals PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\..\\bin
        )