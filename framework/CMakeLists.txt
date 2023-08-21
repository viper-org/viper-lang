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
    "src/parser/ast/global/Function.cpp"
    "src/parser/ast/global/FunctionArgument.cpp"
    "src/parser/ast/global/ExternFunction.cpp"
    "src/parser/ast/statement/ReturnStatement.cpp"
    "src/parser/ast/statement/VariableDeclaration.cpp"
    "src/parser/ast/statement/IfStatement.cpp"
    "src/parser/ast/expression/IntegerLiteral.cpp"
    "src/parser/ast/expression/Variable.cpp"
    "src/parser/ast/expression/BinaryExpression.cpp"
    "src/parser/ast/expression/CallExpression.cpp"

    "src/type/Type.cpp"
    "src/type/IntegerType.cpp"
    "src/type/VoidType.cpp"

    "src/environment/Environment.cpp"
)

set(HEADERS
    "include/lexer/Lexer.h"
    "include/lexer/Token.h"

    "include/parser/Parser.h"
    "include/parser/ast/Node.h"
    "include/parser/ast/global/Function.h"
    "include/parser/ast/global/FunctionArgument.h"
    "include/parser/ast/global/ExternFunction.h"
    "include/parser/ast/statement/ReturnStatement.h"
    "include/parser/ast/statement/VariableDeclaration.h"
    "include/parser/ast/statement/IfStatement.h"
    "include/parser/ast/expression/IntegerLiteral.h"
    "include/parser/ast/expression/Variable.h"
    "include/parser/ast/expression/BinaryExpression.h"
    "include/parser/ast/expression/CallExpression.h"

    "include/type/Type.h"
    "include/type/IntegerType.h"
    "include/type/VoidType.h"

    "include/environment/Environment.h"
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