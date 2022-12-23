#include <llvm/IR/Constant.h>
#include <parsing/ast/expression/string.hh>

namespace Parsing
{
    StringLiteral::StringLiteral(const std::string& value)
        :ASTNode(ASTNodeType::String), _value(value)
    {
        _type = std::make_shared<PointerType>(types.at("int8"));
    }

    void StringLiteral::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<String-Literal>: " << _value;
    }

    std::string StringLiteral::GetValue() const
    {
        return _value;
    }

    llvm::Value* StringLiteral::Emit(llvm::LLVMContext&, llvm::Module&, llvm::IRBuilder<>& builder, std::shared_ptr<Environment>)
    {
        return builder.CreateGlobalStringPtr(_value);
    }
}