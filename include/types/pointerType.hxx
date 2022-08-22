#ifndef VIPER_POINTER_TYPE_HXX
#define VIPER_POINTER_TYPE_HXX
#include <types/type.hxx>

namespace Viper
{
    class PointerType : public Type
    {
    public:
        PointerType(std::shared_ptr<Type> base);

        llvm::Type* GetLLVMType(llvm::LLVMContext& context) const;
    private:
        std::shared_ptr<Type> _base;
    };
}

#endif