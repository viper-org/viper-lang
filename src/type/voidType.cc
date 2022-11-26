#include <type/voidType.hh>

VoidType::VoidType(llvm::LLVMContext& ctx)
    :Type(llvm::Type::getVoidTy(ctx))
{
}

bool VoidType::IsVoidTy() const
{
    return true;
}