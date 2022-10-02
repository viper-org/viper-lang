#include <parsing/ast/expression/binaryExpression.hh>
#include <parsing/ast/expression/integerLiteral.hh>
#include <diagnostics.hh>

namespace Parsing
{
    BinaryExpression::BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token op, std::unique_ptr<ASTNode> rhs)
        :_lhs(std::move(lhs)), _rhs(std::move(rhs))
    {
        _nodeType = ASTNodeType::BinaryExpression;
        switch(op.GetType())
        {
            case Lexing::TokenType::Plus:
                _operator = BinaryOperator::Addition;
                break;
            case Lexing::TokenType::Minus:
                _operator = BinaryOperator::Subtraction;
                break;
            case Lexing::TokenType::Star:
                _operator = BinaryOperator::Multiplication;
                break;
            case Lexing::TokenType::Slash:
                _operator = BinaryOperator::Division;
                break;
            default:
                break;
        }
    }

    std::string BinaryExpression::OperatorToString() const
    {
        switch(_operator)
        {
            case BinaryOperator::Addition:
                return "Addition";
            case BinaryOperator::Subtraction:
                return "Subtraction";
            case BinaryOperator::Multiplication:
                return "Multiplication";
            case BinaryOperator::Division:
                return "Division";
        }
    }

    void BinaryExpression::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Binary-Expression>:\n";
        stream << std::string(indent, ' ') << "Lhs: ";
        _lhs->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "\nOperator: " << OperatorToString() << "\n";
        stream << std::string(indent, ' ') << "Rhs: ";
        _rhs->Print(stream, indent + 2);
    }

    SSA::Value* BinaryExpression::Emit(SSA::Builder& builder)
    {
        SSA::Value* left = _lhs->Emit(builder);
        SSA::Value* right = _rhs->Emit(builder);

        if(SSA::IntegerLiteral* leftI = dynamic_cast<SSA::IntegerLiteral*>(left))
        {
            if(SSA::IntegerLiteral* rightI = dynamic_cast<SSA::IntegerLiteral*>(right))
            {
                long long total;
                switch (_operator)
                {
                    case BinaryOperator::Addition:
                        total = leftI->GetValue() + rightI->GetValue();
                        break;
                    case BinaryOperator::Subtraction:
                        total = leftI->GetValue() - rightI->GetValue();
                        break;
                    case BinaryOperator::Multiplication:
                        total = leftI->GetValue() * rightI->GetValue();
                        break;
                    case BinaryOperator::Division:
                        total = leftI->GetValue() / rightI->GetValue();
                        break;
                }
                delete left;
                delete right;
                return builder.CreateConstantInt(total);
            }
        }
        delete left;
        delete right;
        return nullptr;
    }
}