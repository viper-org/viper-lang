#include <type/arrayType.hh>

ArrayType::ArrayType(unsigned int length, std::shared_ptr<Type> base)
    :Type(base->GetScalarSize() * length, llvm::ArrayType::get(base->GetLLVMType(), length)), _base(base)
{
}

bool ArrayType::IsArrayTy() const
{
    return true;
}

std::shared_ptr<Type> ArrayType::GetBase()
{
    return _base;
}