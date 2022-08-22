#ifndef VIPER_VOID_TYPE_HXX
#define VIPER_VOID_TYPE_HXX
#include <types/type.hxx>

namespace Viper
{
    class VoidType : public Type
    {
    public:
        VoidType();

        llvm::Type* GetLLVMType(llvm::LLVMContext& context) const;
    };
}

#endif