add_executable(palgebra main.cpp)

target_link_libraries(palgebra PUBLIC language)

set_target_properties(palgebra PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\bin
        )

add_subdirectory(Language)