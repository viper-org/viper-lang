#ifndef QRK_AST_PROGRAM_HH
#define QRK_AST_PROGRAM_HH
#include <ast/function.hh>
#include <ast/extern_func.hh>

struct program
{
    std::vector<function_ast> functions;
    std::vector<extern_func> externs;
};

#endif