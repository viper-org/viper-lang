#ifndef QRK_AST_CALL_HH
#define QRK_AST_CALL_HH
#include <ast/expr.hh>

class call_expr : public ast_expr
{
    std::string callee;
    std::vector<std::unique_ptr<ast_expr>> args;
public:
    call_expr(std::string, std::vector<std::unique_ptr<ast_expr>>);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif