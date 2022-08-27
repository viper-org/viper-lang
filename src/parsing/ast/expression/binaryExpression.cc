#include <parsing/ast/expression/binaryExpression.hh>
#include <parsing/ast/expression/integerLiteral.hh>

BinaryExpression::BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token token, std::unique_ptr<ASTNode> rhs)
    :_lhs(std::move(lhs)), _rhs(std::move(rhs))
{
    _nodeType = ASTNodeType::BinaryExpression;
    switch(token.GetType())
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

Codegen::Value* BinaryExpression::Generate(Codegen::Module& module, Codegen::Builder& builder)
{
    Codegen::Value* left = _lhs->Generate(module, builder);
    Codegen::Value* right = _rhs->Generate(module, builder);

    switch(_operator)
    {
        case BinaryOperator::Addition:
            return builder.CreateAdd(left, right);
        case BinaryOperator::Subtraction:
            return builder.CreateSub(left, right);
        case BinaryOperator::Multiplication:
            return builder.CreateMul(left, right);
        case BinaryOperator::Division:
            throw; // TODO: Implement division
    }
    return nullptr;
}