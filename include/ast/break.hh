#ifndef QRK_AST_BREAK_HH
#define QRK_AST_BREAK_HH
#include <ast/expr.hh>

class break_expr : public ast_expr
{
public:
    break_expr();

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif