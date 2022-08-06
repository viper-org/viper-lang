#ifndef QRK_UN_OP_HH
#define QRK_UN_OP_HH
#include <ast/expr.hh>
#include <token.hh>

enum class unary_operand
{
    NEGATION, UNARY_PLUS,

    ADDRESSOF, DEREFERENCE
};

class un_op_expr : public ast_expr
{
    unary_operand operand;
    std::unique_ptr<ast_expr> value;
public:
    un_op_expr(token, std::unique_ptr<ast_expr>);

    std::string operand_to_string() const;

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif