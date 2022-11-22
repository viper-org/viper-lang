#include <type/integerType.hh>
#include <llvm/IR/DerivedTypes.h>

IntegerType::IntegerType(int bits, llvm::LLVMContext& ctx)
    :Type(bits, llvm::Type::getIntNTy(ctx, bits))
{
}

bool IntegerType::IsIntegerTy() const
{
    return true;
}