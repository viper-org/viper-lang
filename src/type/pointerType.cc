#include <type/pointerType.hh>
#include <type/types.hh>

PointerType::PointerType(std::shared_ptr<Type> base)
    :Type(llvm::PointerType::get(base->GetLLVMType(), 0)), _base(base)
{
    if(_base->IsVoidTy())
    {
        _base = types.at("int8");
        _llvmType = llvm::PointerType::get(_base->GetLLVMType(), 0);
    }
}

bool PointerType::IsPointerTy() const
{
    return true;
}

std::shared_ptr<Type> PointerType::GetBase()
{
    return _base;
}