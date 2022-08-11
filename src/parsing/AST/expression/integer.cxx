#include <llvm/ADT/APInt.h>
#include <parsing/AST/expression/integer.hxx>
#include <llvm/IR/Constants.h>

namespace Viper
{
    namespace Parsing
    {
        IntegerLiteral::IntegerLiteral(int value)
            :_value(value)
        {
            _type = ASTNodeType::Integer;
        }

        void IntegerLiteral::Print(std::ostream& stream) const
        {
            stream << "<Integer-Literal>: " << _value;
        }

        llvm::Value* IntegerLiteral::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>&, llvm::Module&, std::shared_ptr<Environment>)
        {
            return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), _value);
        }
    }
}