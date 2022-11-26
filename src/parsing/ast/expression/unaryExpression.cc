#include <llvm/IR/Instructions.h>
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
            case Lexing::TokenType::Bang:
                _operator = UnaryOperator::LogicalNegation;
                break;
            case Lexing::TokenType::Hash:
                _operator = UnaryOperator::AddressOf;
                break;
            case Lexing::TokenType::Asperand:
                _operator = UnaryOperator::Indirection;
                _type = _operand->GetType()->GetBase();
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

    llvm::Value* UnaryExpression::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        llvm::Value* operand = _operand->Emit(ctx, mod, builder, scope);

        switch(_operator)
        {
            case UnaryOperator::Indirection:
                return builder.CreateLoad(operand->getType()->getPointerElementType(), operand);
            case UnaryOperator::AddressOf:
            {
                llvm::Value* ptr = llvm::getPointerOperand(operand);

                llvm::Instruction* inst = static_cast<llvm::Instruction*>(operand);
                inst->eraseFromParent();
                
                return ptr;
            }
            case UnaryOperator::IntegralNegation:
                return builder.CreateNeg(operand);
            case UnaryOperator::LogicalNegation:
                return builder.CreateNot(operand);
            default:
                return nullptr;
        }
    }
}