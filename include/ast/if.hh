#ifndef QRK_AST_IF_HH
#define QRK_AST_IF_HH
#include <ast/expr.hh>

class if_expr : public ast_expr
{
    std::unique_ptr<ast_expr> condition;
    std::unique_ptr<ast_expr> body;
    std::unique_ptr<ast_expr> else_body;
    std::shared_ptr<scope> env;
    std::shared_ptr<scope> else_env;
public:
    if_expr(std::unique_ptr<ast_expr>, std::unique_ptr<ast_expr>, std::unique_ptr<ast_expr>, std::shared_ptr<scope>, std::shared_ptr<scope>);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif