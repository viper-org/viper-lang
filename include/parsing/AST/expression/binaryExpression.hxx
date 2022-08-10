#ifndef QUARK_AST_BINARYEXPRESSION_HXX
#define QUARK_AST_BINARYEXPRESSION_HXX
#include <parsing/AST/astNode.hxx>
#include <lexing/token.hxx>
#include <memory>

namespace Viper
{
    namespace Parsing
    {
        enum class BinaryOperator
        {
            Plus, Minus, Multiply, Divide
        };

        class BinaryExpression : public ASTNode
        {
        public:
            BinaryExpression(std::unique_ptr<ASTNode> lhs, BinaryOperator op, std::unique_ptr<ASTNode> rhs);
            BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token  op, std::unique_ptr<ASTNode> rhs);

            void Print(std::ostream& stream) const override;
        private:
            std::unique_ptr<ASTNode> _lhs;
            BinaryOperator           _operator;
            std::unique_ptr<ASTNode> _rhs;

            std::string OperatorToString() const;
        };
    }
}

#endif