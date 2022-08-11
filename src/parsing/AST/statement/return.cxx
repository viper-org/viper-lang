#include <parsing/AST/statement/return.hxx>

namespace Viper
{
    namespace Parsing
    {
        ReturnStatement::ReturnStatement(std::unique_ptr<ASTNode> value)
            :_value(std::move(value))
        {
            _type = ASTNodeType::Return;
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
            if(_value)
                return builder.CreateRet(_value->Generate(context, builder, module, scope));

            return builder.CreateRet(nullptr);
        }
    }
}