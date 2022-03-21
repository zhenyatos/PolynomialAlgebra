add_library(language SHARED Lexer.cpp Parser.cpp Interpreter.cpp nodes.cpp functions.cpp polynodes.cpp types.cpp)

target_include_directories(language PUBLIC include)

target_link_libraries(language PUBLIC numbers polynomials)

set_target_properties(language PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ..\\..\\..\\bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ..\\..\\..\\bin
        )