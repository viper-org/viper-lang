#include <ast/expr.hh>

std::ostream& operator<<(std::ostream& stream, ast_expr& expr)
{
    expr.print(stream);
    return stream;
}