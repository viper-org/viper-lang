#ifndef VIPER_AST_UNARY_HH
#define VIPER_AST_UNARY_HH
#include <parsing/ast/astNode.hh>
#include <lexing/token.hh>

namespace Parsing
{
    enum class UnaryOperator
    {
        IntegralNegation,
        LogicalNegation,

        AddressOf,
        Indirection,
    };

    class UnaryExpression : public ASTNode
    {
    public:
        UnaryExpression(std::unique_ptr<ASTNode> operand, Lexing::Token op);

        void Print(std::ostream& stream, int indent) const override;

        SSA::Value* Emit(SSA::Builder& builder, Environment* scope, bool isStatement) override;
    private:
        std::unique_ptr<ASTNode>_operand;
        UnaryOperator _operator;

        std::string OperatorToString() const;
    };
}

#endif