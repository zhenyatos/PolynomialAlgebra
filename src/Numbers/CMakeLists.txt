add_library(numbers SHARED
        Integer.cpp)

target_include_directories(numbers PUBLIC include)

set_target_properties(numbers PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )

