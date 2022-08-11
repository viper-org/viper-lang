#ifndef VIPER_INTEGER_TYPE_HXX
#define VIPER_INTEGER_TYPE_HXX
#include <types/type.hxx>

namespace Viper
{
    class IntegerType : public Type
    {
    public:
        IntegerType(int bits);

        llvm::Type* GetLLVMType(llvm::LLVMContext& context) const;
    };
}

#endif