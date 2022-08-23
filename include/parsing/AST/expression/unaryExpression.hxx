#ifndef VIPER_AST_UNARY_EXPRESSION_HXX
#define VIPER_AST_UNARY_EXPRESSION_HXX
#include <parsing/AST/astNode.hxx>
#include <lexing/token.hxx>
#include <memory>

namespace Viper
{
    namespace Parsing
    {
        enum class UnaryOperator
        {
            PrefixIncrement,  PrefixDecrement,
            PostfixIncrement, PostfixDecrement,

            LogicalNegation,

            IntegralNegation,

            Indirection, AddressOf,
        };

        class UnaryExpression : public ASTNode
        {
        public:
            UnaryExpression(std::unique_ptr<ASTNode> operand, UnaryOperator op);
            UnaryExpression(std::unique_ptr<ASTNode> operand, Lexing::Token  op);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        private:
            std::unique_ptr<ASTNode> _operand;
            UnaryOperator            _operator;

            std::string OperatorToString() const;
        };
    }
}

#endif