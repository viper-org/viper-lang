#include <llvm/IR/DerivedTypes.h>
#include <types/arrayType.hxx>

namespace Viper
{
    ArrayType::ArrayType(unsigned int length, std::shared_ptr<Type> base)
        :Type("[" + std::to_string(length) + " x " + base->GetLLVMName().data() + "]", "[" + std::to_string(length) + " x " + base->GetLLVMName().data() + "]", length * base->GetSize()), _base(base)
    {
    }

    llvm::Type* ArrayType::GetLLVMType(llvm::LLVMContext& context) const
    {
        return llvm::ArrayType::get(_base->GetLLVMType(context), _size / _base->GetSize());
    }
}