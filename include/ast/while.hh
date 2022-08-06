#ifndef QRK_AST_WHILE_HH
#define QRK_AST_WHILE_HH
#include <ast/expr.hh>
#include <globals.hh>

class while_expr : public ast_expr
{
    std::unique_ptr<ast_expr> cond;
    std::unique_ptr<ast_expr> body;

    std::shared_ptr<scope> env;

    bool is_do_while;
public:
    while_expr(std::unique_ptr<ast_expr>, std::unique_ptr<ast_expr>, std::shared_ptr<scope>, bool);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif