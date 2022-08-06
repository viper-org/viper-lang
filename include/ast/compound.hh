#ifndef QRK_AST_COMPOUND_HH
#define QRK_AST_COMPOUND_HH
#include <ast/expr.hh>

class compound_expr : public ast_expr
{
    std::vector<std::unique_ptr<ast_expr>> expressions;

    std::shared_ptr<scope> env;
public:
    compound_expr(std::vector<std::unique_ptr<ast_expr>>, std::shared_ptr<scope>);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif