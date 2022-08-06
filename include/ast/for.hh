#ifndef QRK_AST_FOR_HH
#define QRK_AST_FOR_HH
#include <ast/expr.hh>

class for_expr : public ast_expr
{
    std::unique_ptr<ast_expr> init;
    std::unique_ptr<ast_expr> cond;
    std::unique_ptr<ast_expr> iter;

    std::shared_ptr<scope> env;

    std::unique_ptr<ast_expr> body;
public:
    for_expr(std::unique_ptr<ast_expr>, std::unique_ptr<ast_expr>, std::unique_ptr<ast_expr>, std::shared_ptr<scope>, std::unique_ptr<ast_expr>);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif