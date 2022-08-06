#ifndef QRK_AST_VAR_HH
#define QRK_AST_VAR_HH
#include <ast/expr.hh>

class var_expr : public ast_expr
{
    std::string name;
public:
    var_expr(std::string);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;

    std::string get_name() { return name; }
};

#endif