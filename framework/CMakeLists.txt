cmake_minimum_required(VERSION 3.26)

include(FetchContent)
FetchContent_Declare(vipir
    GIT_REPOSITORY https://github.com/viper-org/vipir
    GIT_TAG master
)
FetchContent_MakeAvailable(vipir)

set(SOURCES
    "src/lexer/Lexer.cpp"
    "src/lexer/Token.cpp"

    "src/parser/Parser.cpp"
)

set(HEADERS
    "include/lexer/Lexer.h"
    "include/lexer/Token.h"

    "include/parser/Parser.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(viper-framework-viper-framework ${SOURCES} ${HEADERS})
add_library(viper::framework ALIAS viper-framework-viper-framework)
target_include_directories(viper-framework-viper-framework
    PUBLIC
        include
)
target_compile_features(viper-framework-viper-framework PUBLIC cxx_std_20)
target_link_libraries(viper-framework-viper-framework vipir)