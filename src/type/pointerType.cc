#include <type/pointerType.hh>

PointerType::PointerType(std::shared_ptr<Type> base)
    :Type(64, llvm::PointerType::get(base->GetLLVMType(), 0)), _base(base)
{
}

bool PointerType::IsPointerTy() const
{
    return true;
}

int PointerType::GetPrimitiveSize() const
{
    return _llvmType->getPrimitiveSizeInBits();
}

std::shared_ptr<Type> PointerType::GetBase()
{
    return _base;
}