add_library(parser SHARED Parser.cpp)

target_include_directories(parser PUBLIC include)

set_target_properties(parser PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )

set(CMAKE_SHARED_LIBRARY_PREFIX "")