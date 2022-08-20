#ifndef VIPER_AST_BINARYEXPRESSION_HXX
#define VIPER_AST_BINARYEXPRESSION_HXX
#include <parsing/AST/astNode.hxx>
#include <lexing/token.hxx>
#include <memory>

namespace Viper
{
    namespace Parsing
    {
        enum class BinaryOperator
        {
            Plus, Minus, Multiply, Divide,

            Assignment,

            Equals, NotEquals,
            And, Or
        };

        class BinaryExpression : public ASTNode
        {
        public:
            BinaryExpression(std::unique_ptr<ASTNode> lhs, BinaryOperator op, std::unique_ptr<ASTNode> rhs);
            BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token  op, std::unique_ptr<ASTNode> rhs);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope) override;
        private:
            std::unique_ptr<ASTNode> _lhs;
            BinaryOperator           _operator;
            std::unique_ptr<ASTNode> _rhs;

            std::string OperatorToString() const;
        };
    }
}

#endif