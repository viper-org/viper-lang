#include <parsing/AST/expression/integer.hxx>
#include <llvm/IR/Constants.h>

namespace Viper
{
    namespace Parsing
    {
        IntegerLiteral::IntegerLiteral(int value)
            :_value(value)
        {
            _nodeType = ASTNodeType::Integer;
            _type = types.at("i32");
        }

        void IntegerLiteral::Print(std::ostream& stream) const
        {
            stream << "<Integer-Literal>: " << _value;
        }

        llvm::Value* IntegerLiteral::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>&, llvm::Module&, std::shared_ptr<Environment>, std::vector<CodegenFlag>)
        {
            return llvm::ConstantInt::get(_type->GetLLVMType(context), _value);
        }
    }
}