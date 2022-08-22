#include <llvm/IR/DerivedTypes.h>
#include <types/pointerType.hxx>

namespace Viper
{
    PointerType::PointerType(std::shared_ptr<Type> base)
        :Type(base->GetName().data() + std::string("*"), base->GetName().data() + std::string("*"), 8), _base(base)
    {
    }

    llvm::Type* PointerType::GetLLVMType(llvm::LLVMContext& context) const
    {
        return llvm::PointerType::get(_base->GetLLVMType(context), 0);
    }
}