add_executable(app main.cpp)

target_link_libraries(app PUBLIC numbers polynomials parser)

set_target_properties(app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\bin
        )

add_subdirectory(Parser)