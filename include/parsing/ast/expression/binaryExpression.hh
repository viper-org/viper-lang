#ifndef VIPER_AST_BINARY_HH
#define VIPER_AST_BINARY_HH
#include <parsing/ast/astNode.hh>
#include <lexing/token.hh>

namespace Parsing
{
    enum class BinaryOperator
    {
        Addition, Subtraction,
        Multiplication, Division,

        Equal, NotEqual,
        LessThan, GreaterThan,

        Assignment,
    };

    class BinaryExpression : public ASTNode
    {
    public:
        BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token op, std::unique_ptr<ASTNode> rhs);

        void Print(std::ostream& stream, int indent) const override;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder) override;
    private:
        std::unique_ptr<ASTNode>_lhs;
        BinaryOperator _operator;
        std::unique_ptr<ASTNode> _rhs;

        std::string OperatorToString() const;
    };
}

#endif