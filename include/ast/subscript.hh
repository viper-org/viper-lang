#ifndef QRK_AST_SUBSCRIPT_HH
#define QRK_AST_SUBSCRIPT_HH
#include <ast/expr.hh>

class subscript_expr : public ast_expr
{
    std::unique_ptr<ast_expr> operand;
    std::unique_ptr<ast_expr> index;
public:
    subscript_expr(std::unique_ptr<ast_expr>, std::unique_ptr<ast_expr>);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
    llvm::Value* no_load_codegen(std::shared_ptr<scope>) const;
};

#endif