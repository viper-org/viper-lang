#ifndef VIPER_ARRAY_TYPE_HXX
#define VIPER_ARRAY_TYPE_HXX
#include <types/type.hxx>

namespace Viper
{
    class ArrayType : public Type
    {
    public:
        ArrayType(unsigned int length, std::shared_ptr<Type> base);

        llvm::Type* GetLLVMType(llvm::LLVMContext& context) const;
    private:
        std::shared_ptr<Type> _base;
    };
}

#endif