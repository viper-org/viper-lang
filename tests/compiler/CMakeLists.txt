cmake_minimum_required (VERSION 3.26)

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    "source/main.cpp"
    "source/Test.cpp"

    "source/lexerTests/LexerTests.cpp"

    "source/parserTests/ParserTests.cpp"
)

set(HEADERS
    "include/ViperAssert.h"
    "include/Test.h"
)

add_executable(viper-compiler-tests ${SOURCES} ${HEADERS})
add_executable(viper::tests ALIAS viper-compiler-tests)

target_include_directories(viper-framework-viper-framework PUBLIC include)
target_compile_features(viper-compiler-tests PUBLIC cxx_std_20)
target_link_libraries(viper-compiler-tests viper::framework)