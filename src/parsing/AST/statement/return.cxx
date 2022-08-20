#include <iostream>
#include <parsing/AST/statement/return.hxx>

namespace Viper
{
    namespace Parsing
    {
        ReturnStatement::ReturnStatement(std::unique_ptr<ASTNode> value)
            :_value(std::move(value))
        {
            _nodeType = ASTNodeType::Return;
        }

        void ReturnStatement::Print(std::ostream& stream) const
        {
            stream << "<Return-Statement>:\nValue: ";
            if(_value)
                _value->Print(stream);
            else
                stream << "None";
        }

        llvm::Value* ReturnStatement::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope)
        {
            llvm::Type* returnType = builder.GetInsertBlock()->getParent()->getReturnType();
            if(_value)
            {
                llvm::Value* generatedValue = _value->Generate(context, builder, module, scope);
                if(generatedValue->getType() != returnType)
                    generatedValue = Type::Convert(generatedValue, returnType, builder);
                return builder.CreateRet(generatedValue);
            }

            return builder.CreateRet(nullptr);
        }
    }
}