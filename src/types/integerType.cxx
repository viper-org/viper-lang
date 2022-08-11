#include <types/integerType.hxx>
#include <llvm/IR/DerivedTypes.h>

namespace Viper
{
    IntegerType::IntegerType(int bits)
        :Type(std::string("i") + std::to_string(bits), std::string("i") + std::to_string(bits), bits / 8)
    {
    }

    llvm::Type* IntegerType::GetLLVMType(llvm::LLVMContext& context) const
    {
        return llvm::Type::getIntNTy(context, _size * 8);
    }
}