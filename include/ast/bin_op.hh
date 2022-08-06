#ifndef QRK_AST_BIN_OP_HH
#define QRK_AST_BIN_OP_HH
#include <ast/expr.hh>
#include <token.hh>

enum class binary_operand
{
    PLUS, MINUS, MULTIPLY, DIVIDE,

    EQUAL, NOT_EQUAL,
    LESS, GREATER,

    LESS_EQUAL, GREATER_EQUAL,

    ASSIGNMENT, INCREMENT_ASSIGN, DECREMENT_ASSIGN,
};

class bin_op_expr : public ast_expr
{
    binary_operand operand;
    std::unique_ptr<ast_expr> lhs;
    std::unique_ptr<ast_expr> rhs;
public:
    bin_op_expr(token, std::unique_ptr<ast_expr>, std::unique_ptr<ast_expr>);

    std::string operand_to_string() const;

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif