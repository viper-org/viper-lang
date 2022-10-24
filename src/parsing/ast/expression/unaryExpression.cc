#include <iostream>
#include <parsing/ast/expression/unaryExpression.hh>
#include <parsing/ast/expression/variable.hh>
#include <environment.hh>

namespace Parsing
{
    UnaryExpression::UnaryExpression(std::unique_ptr<ASTNode> operand, Lexing::Token op)
        :ASTNode(ASTNodeType::BinaryExpression), _operand(std::move(operand))
    {
        switch(op.GetType())
        {
            case Lexing::TokenType::Minus:
                _operator = UnaryOperator::IntegralNegation;
                break;
            case Lexing::TokenType::Hash:
                _operator = UnaryOperator::AddressOf;
                break;
            case Lexing::TokenType::Asperand:
                _operator = UnaryOperator::Indirection;
                break;
            default:
                break;
        }
    }

    std::string UnaryExpression::OperatorToString() const
    {
        switch(_operator)
        {
            case UnaryOperator::IntegralNegation:
                return "IntegralNegation";
            case UnaryOperator::LogicalNegation:
                return "LogicalNegation";
            case UnaryOperator::AddressOf:
                return "AddressOf";
            case UnaryOperator::Indirection:
                return "Indirection";
        }
        return "";
    }

    void UnaryExpression::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Unary-Expression>:\n";
        stream << std::string(indent, ' ') << "Operand: ";
        _operand->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "\nOperator: " << OperatorToString() << "\n";
    }

    SSA::Value* UnaryExpression::Emit(SSA::Builder& builder, Environment* scope, bool)
    {
        if(_operator == UnaryOperator::AddressOf)
        {
            if(Variable* operand = dynamic_cast<Variable*>(_operand.get()))
            {
                SSA::AllocaInst* alloca = scope->FindNamedValue(operand->GetName());
                return alloca;
            }
        }
        SSA::Value* operand = _operand->Emit(builder, scope);

        SSA::Value* retval = nullptr;
        switch(_operator)
        {
            case UnaryOperator::Indirection:
                retval = builder.CreateLoad(operand);
                break;
            default:
                break;
        }

        return retval;
    }
}