#ifndef QRK_AST_LABEL_HH
#define QRK_AST_LABEL_HH
#include <ast/expr.hh>

class label_expr : public ast_expr
{
    std::string label;
public:
    label_expr(std::string);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif