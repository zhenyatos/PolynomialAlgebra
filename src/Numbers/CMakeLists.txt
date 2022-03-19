add_library(numbers SHARED
        Integer.cpp
        Rational.cpp
        Modular.cpp)

target_include_directories(numbers PUBLIC include)

set_target_properties(numbers PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )

add_subdirectory(example)