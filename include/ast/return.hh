#ifndef QRK_AST_RETURN_HH
#define QRK_AST_RETURN_HH
#include <ast/expr.hh>

class return_stmt : public ast_expr
{
    std::unique_ptr<ast_expr> value;
public:
    return_stmt(std::unique_ptr<ast_expr>);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif