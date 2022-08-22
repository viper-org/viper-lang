#include <iostream>
#include <parsing/AST/expression/subscript.hxx>
#include <parsing/AST/expression/variable.hxx>

namespace Viper
{
    namespace Parsing
    {
        SubscriptExpression::SubscriptExpression(std::unique_ptr<ASTNode> operand, std::unique_ptr<ASTNode> index)
            :_operand(std::move(operand)), _index(std::move(index))
        {
            _nodeType = ASTNodeType::SubscriptExpression;
        }

        void SubscriptExpression::Print(std::ostream& stream) const
        {
            stream << "<Subscript-Expression>:\nOperand: ";
            _operand->Print(stream);
            stream << "\nIndex: ";
            _index->Print(stream);
        }

        llvm::Value* SubscriptExpression::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope)
        {
            if(_operand->GetNodeType() == ASTNodeType::Variable)
            {
                Variable* left = static_cast<Variable*>(_operand.get());

                llvm::AllocaInst* alloca = FindNamedValue(left->GetName(), scope);
                llvm::Value* indexCodegen = Type::Convert(_index->Generate(context, builder, module, scope), types.at("i64")->GetLLVMType(context), builder);
                return builder.CreateInBoundsGEP(alloca->getAllocatedType(), alloca, { llvm::ConstantInt::get(types.at("i64")->GetLLVMType(context), 0), indexCodegen }, "subscript");
            }
            return nullptr;
        }
    }
}