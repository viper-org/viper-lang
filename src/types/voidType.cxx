#include <types/voidType.hxx>

namespace Viper
{
    VoidType::VoidType()
        :Type("void", "void", 0)
    {
    }

    llvm::Type* VoidType::GetLLVMType(llvm::LLVMContext& context) const
    {
        return llvm::Type::getVoidTy(context);
    }
}