cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "source/lexer/Lexer.cpp"
    "source/lexer/Token.cpp"

    "source/symbol/Scope.cpp"

    "source/parser/Parser.cpp"
    "source/parser/ast/global/Function.cpp"
    "source/parser/ast/statement/ReturnStatement.cpp"
    "source/parser/ast/expression/IntegerLiteral.cpp"

    "source/diagnostic/Diagnostic.cpp"
)

set(HEADERS
    "include/lexer/Lexer.h"
    "include/lexer/SourceLocation.h"
    "include/lexer/Token.h"

    "include/symbol/Scope.h"

    "include/parser/Parser.h"
    "include/parser/ast/ASTNode.h"
    "include/parser/ast/global/Function.h"
    "include/parser/ast/statement/ReturnStatement.h"
    "include/parser/ast/expression/IntegerLiteral.h"

    "include/diagnostic/Diagnostic.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(viper-framework-viper-framework ${SOURCES} ${HEADERS})
add_library(viper::framework ALIAS viper-framework-viper-framework)

target_include_directories(viper-framework-viper-framework PUBLIC include)
target_compile_features(viper-framework-viper-framework PUBLIC cxx_std_20)