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

        New,
    };

    class UnaryExpression : public ASTNode
    {
    friend class CallExpr;
    public:
        UnaryExpression(std::unique_ptr<ASTNode> operand, Lexing::Token op);

        void Print(std::ostream& stream, int indent) const override;

        void AssignType();

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::unique_ptr<ASTNode>_operand;
        UnaryOperator _operator;

        std::string OperatorToString() const;
    };
}

#endif