#include <type/voidType.hh>

VoidType::VoidType(llvm::LLVMContext& ctx)
    :Type(0, llvm::Type::getVoidTy(ctx))
{
}

bool VoidType::IsVoidTy() const
{
    return true;
}