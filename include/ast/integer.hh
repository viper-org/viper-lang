#ifndef QRK_AST_INTEGER_HH
#define QRK_AST_INTEGER_HH
#include <ast/expr.hh>

class integer_expr : public ast_expr
{
    int value;
public:
    integer_expr(int);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif