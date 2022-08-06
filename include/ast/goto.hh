#ifndef QRK_AST_GOTO_HH
#define QRK_AST_GOTO_HH
#include <ast/expr.hh>

class goto_expr : public ast_expr
{
    std::string label;
public:
    goto_expr(std::string);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif