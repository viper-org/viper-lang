#include <parsing/ast/expression/integerLiteral.hh>
#include <llvm/IR/Constants.h>

namespace Parsing
{
    IntegerLiteral::IntegerLiteral(long long value)
        :ASTNode(ASTNodeType::Integer), _value(value)
    {
        _type = types.at("int64");
    }

    void IntegerLiteral::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Integer-Literal>: " <<_value;
    }

    long long IntegerLiteral::GetValue() const
    {
        return _value;
    }

    llvm::Value* IntegerLiteral::Emit(llvm::LLVMContext& ctx, llvm::Module&, llvm::IRBuilder<>&, std::shared_ptr<Environment>)
    {
        return llvm::ConstantInt::get(ctx, llvm::APInt(32, _value));
    }
}