add_executable(app main.cpp)

target_link_libraries(app PUBLIC numbers)

set_target_properties(app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\bin
        )