#include <parsing/ast/statement/return.hh>

namespace Parsing
{
    ReturnStatement::ReturnStatement(std::unique_ptr<ASTNode> value, std::shared_ptr<Type> returnType)
        :ASTNode(ASTNodeType::ReturnStatement), _value(std::move(value)), _returnType(returnType)
    {
    }

    void ReturnStatement::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Return-Statement>";
        if(_value)
        {
            stream << ":\n" << std::string(indent, ' ') << "Value:\n";
            _value->Print(stream, indent + 2);
        }
    }

    llvm::Value* ReturnStatement::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        llvm::Value* value = nullptr;
        if(_value)
        {
            value = _value->Emit(ctx, mod, builder, scope);

            value = Type::Convert(value, _returnType->GetLLVMType(), builder);
        }

        return builder.CreateRet(value);
    }
}