#include <llvm/IR/Instructions.h>
#include <parsing/ast/expression/unaryExpression.hh>
#include <parsing/ast/expression/variable.hh>
#include <parsing/ast/expression/call.hh>
#include <environment.hh>

namespace Parsing
{
    UnaryExpression::UnaryExpression(std::unique_ptr<ASTNode> operand, Lexing::Token op)
        :ASTNode(ASTNodeType::UnaryExpression), _operand(std::move(operand))
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
                break;
            case Lexing::TokenType::New:
                _operator = UnaryOperator::New;
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
            case UnaryOperator::New:
                return "New";
        }
        return "";
    }

    void UnaryExpression::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Unary-Expression>:\n";
        stream << std::string(indent, ' ') << "Operand:\n";
        _operand->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "\nOperator: " << OperatorToString() << "\n";
    }

    void UnaryExpression::AssignType()
    {
        switch(_operator)
        {
            case UnaryOperator::Indirection:
                _type = _operand->GetType()->GetBase();
                break;
            case UnaryOperator::New:
                _type = types.at(static_cast<Variable*>(_operand.get())->GetName());
            default:
                break;
        }
    }

    llvm::Value* UnaryExpression::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        llvm::Value* operand = _operand->Emit(ctx, mod, builder, scope);
        AssignType();

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
            case UnaryOperator::New:
                return operand;
            default:
                return nullptr;
        }
    }
}