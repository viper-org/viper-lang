#ifndef VIPER_POINTER_TYPE_HH
#define VIPER_POINTER_TYPE_HH
#include <type/type.hh>

class PointerType : public Type
{
public:
    PointerType(std::shared_ptr<Type> base);
    bool IsPointerTy() const override;

    int GetPrimitiveSize() const override;

    std::shared_ptr<Type> GetBase() override;
private:
    std::shared_ptr<Type> _base;
};

#endif