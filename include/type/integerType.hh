#ifndef VIPER_INTEGER_TYPE_HH
#define VIPER_INTEGER_TYPE_HH
#include <type/type.hh>
#include <llvm/IR/LLVMContext.h>

class IntegerType : public Type
{
public:
    IntegerType(int bits, llvm::LLVMContext& ctx);
    bool IsIntegerTy() const override;
};

#endif