#include <parsing/ast/expression/binaryExpression.hh>
#include <parsing/ast/expression/integerLiteral.hh>
#include <parsing/ast/expression/variable.hh>
#include <environment.hh>
#include <diagnostics.hh>

namespace Parsing
{
    BinaryExpression::BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token op, std::unique_ptr<ASTNode> rhs)
        :ASTNode(ASTNodeType::BinaryExpression), _lhs(std::move(lhs)), _rhs(std::move(rhs))
    {
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
            case Lexing::TokenType::DoubleEquals:
                _operator = BinaryOperator::Equal;
                break;
            case Lexing::TokenType::BangEquals:
                _operator = BinaryOperator::NotEqual;
                break;
            case Lexing::TokenType::LeftAngleBracket:
                _operator = BinaryOperator::LessThan;
                break;
            case Lexing::TokenType::RightAngleBracket:
                _operator = BinaryOperator::GreaterThan;
                break;
            case Lexing::TokenType::Equals:
                _operator = BinaryOperator::Assignment;
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
            case BinaryOperator::Equal:
                return "Equal";
            case BinaryOperator::NotEqual:
                return "NotEqual";
            case BinaryOperator::LessThan:
                return "LessThan";
            case BinaryOperator::GreaterThan:
                return "GreaterThan";
            case BinaryOperator::Assignment:
                return "Assignment";
        }
        return "";
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

    SSA::Value* BinaryExpression::Emit(SSA::Builder& builder, Environment* scope, bool)
    {
        SSA::Value* right = _rhs->Emit(builder, scope);
        if(_operator == BinaryOperator::Assignment)
        {
            if(Variable* left = dynamic_cast<Variable*>(_lhs.get()))
            {
                SSA::AllocaInst* alloca = scope->FindNamedValue(left->GetName());
                builder.CreateStore(alloca, right);
                return right;
            }
        }

        SSA::Value* left = _lhs->Emit(builder, scope);

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
                    default:
                        throw; // Unreachable
                }
                delete left;
                delete right;
                return builder.CreateConstantInt(total);
            }
        }
        SSA::Value* retval = nullptr;
        switch(_operator)
        {
            case BinaryOperator::Addition:
                retval = builder.CreateAdd(left, right);
                break;
            case BinaryOperator::Subtraction:
                retval = builder.CreateSub(left, right);
                break;
            case BinaryOperator::Multiplication:
                retval = builder.CreateMul(left, right);
                break;
            case BinaryOperator::Division:
                retval = builder.CreateDiv(left, right);
                break;
            case BinaryOperator::Equal:
                retval = builder.CreateCmp(SSA::Instruction::EQ, left, right);
                break;
            case BinaryOperator::NotEqual:
                retval = builder.CreateCmp(SSA::Instruction::NE, left, right);
                break;
            case BinaryOperator::LessThan:
                retval = builder.CreateCmp(SSA::Instruction::LT, left, right);
                break;
            case BinaryOperator::GreaterThan:
                retval = builder.CreateCmp(SSA::Instruction::GT, left, right);
                break;
            case BinaryOperator::Assignment:
                builder.CreateStore(left, right);
                retval = left;
            default:
                break;
        }

        return retval;
    }
}