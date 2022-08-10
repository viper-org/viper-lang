#include <parsing/AST/expression/binaryExpression.hxx>

namespace Viper
{
    namespace Parsing
    {
        BinaryExpression::BinaryExpression(std::unique_ptr<ASTNode> lhs, BinaryOperator op, std::unique_ptr<ASTNode> rhs)
            :_lhs(std::move(lhs)), _operator(op), _rhs(std::move(rhs))
        {
        }

        BinaryExpression::BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token op, std::unique_ptr<ASTNode> rhs)
            :_lhs(std::move(lhs)), _rhs(std::move(rhs))
        {
            switch(op.getType())
            {
                case Lexing::TokenType::Plus:
                    _operator = BinaryOperator::Plus;
                    break;
                case Lexing::TokenType::Minus:
                    _operator = BinaryOperator::Minus;
                    break;
                case Lexing::TokenType::Star:
                    _operator = BinaryOperator::Multiply;
                    break;
                case Lexing::TokenType::Slash:
                    _operator = BinaryOperator::Divide;
                    break;
                default:
                    // TODO: Compiler error
                    throw;
            }
        }

        void BinaryExpression::Print(std::ostream& stream) const
        {
            stream << "<Binary-Expression>:\nLhs: ";
            _lhs->Print(stream);
            stream << "\nOperator: " << OperatorToString() << "\nRhs: ";
            _rhs->Print(stream);
        }

        std::string BinaryExpression::OperatorToString() const
        {
            switch(_operator)
            {
                case BinaryOperator::Plus:
                    return "PLUS";
                case BinaryOperator::Minus:
                    return "MINUS";
                case BinaryOperator::Multiply:
                    return "MULTIPLY";
                case BinaryOperator::Divide:
                    return "DIVIDE";
            }
        }
    }
}