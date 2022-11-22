#ifndef VIPER_VOID_TYPE_HH
#define VIPER_VOID_TYPE_HH
#include <type/type.hh>

class VoidType : public Type
{
public:
    VoidType(llvm::LLVMContext& ctx);
    bool IsVoidTy() const override;
};

#endif