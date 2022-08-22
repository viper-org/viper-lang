#include <iostream>
#include <parsing/AST/expression/unaryExpression.hxx>

namespace Viper
{
    namespace Parsing
    {
        UnaryExpression::UnaryExpression(std::unique_ptr<ASTNode> operand, UnaryOperator op)
            :_operand(std::move(operand)), _operator(op)
        {
            _nodeType = ASTNodeType::UnaryExpression;
        }

        UnaryExpression::UnaryExpression(std::unique_ptr<ASTNode> operand, Lexing::Token op)
            :_operand(std::move(operand))
        {
            switch(op.getType())
            {
                case Lexing::TokenType::Bang:
                    _operator = UnaryOperator::LogicalNegation;
                    break;
                case Lexing::TokenType::Minus:
                    _operator = UnaryOperator::IntegralNegation;
                    break;
                case Lexing::TokenType::Increment:
                    _operator = UnaryOperator::PrefixIncrement;
                    break;
                case Lexing::TokenType::Decrement:
                    _operator = UnaryOperator::PrefixDecrement;
                    break;
                default: // This should never be reached
                    break;
            }
        }

        void UnaryExpression::Print(std::ostream& stream) const
        {
            stream << "<Unary-Expression>:\nLhs: ";
            _operand->Print(stream);
            stream << "\nOperator: " << OperatorToString();
        }

        std::string UnaryExpression::OperatorToString() const
        {
            switch(_operator)
            {
                case UnaryOperator::PrefixIncrement:
                    return "PrefixIncrement";
                case UnaryOperator::PrefixDecrement:
                    return "PrefixDecrement";
                case UnaryOperator::PostfixIncrement:
                    return "PostfixIncrement";
                case UnaryOperator::PostfixDecrement:
                    return "PostfixDecrement";
                case UnaryOperator::LogicalNegation:
                    return "LogicalNegation";
                case UnaryOperator::IntegralNegation:
                    return "IntegralNegation";
            }
        }

        llvm::Value* UnaryExpression::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags)
        {
            llvm::Value* operandCodegen = _operand->Generate(context, builder, module, scope, flags);
            switch(_operator)
            {
                case UnaryOperator::LogicalNegation:
                {
                    operandCodegen = Type::Convert(operandCodegen, types.at("bool")->GetLLVMType(context), builder);
                    return builder.CreateNot(operandCodegen, "not");
                }
                case UnaryOperator::IntegralNegation:
                    return builder.CreateNeg(operandCodegen, "neg");
                
                case UnaryOperator::PrefixIncrement:
                    return builder.CreateAdd(operandCodegen, llvm::ConstantInt::get(operandCodegen->getType(), 1));
                case UnaryOperator::PrefixDecrement:
                    return builder.CreateAdd(operandCodegen, llvm::ConstantInt::get(operandCodegen->getType(), -1));

                case UnaryOperator::PostfixIncrement:
                {
                    builder.CreateAdd(operandCodegen, llvm::ConstantInt::get(operandCodegen->getType(), 1));
                    return operandCodegen;
                }
                case UnaryOperator::PostfixDecrement:
                {
                    builder.CreateAdd(operandCodegen, llvm::ConstantInt::get(operandCodegen->getType(), -1));
                    return operandCodegen;
                }
                default: // This should never be reached
                    break;
            }

            return nullptr;
        }
    }
}