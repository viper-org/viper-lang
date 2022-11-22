#ifndef VIPER_ARRAY_TYPE_HH
#define VIPER_ARRAY_TYPE_HH
#include <type/type.hh>

class ArrayType : public Type
{
public:
    ArrayType(unsigned int length, std::shared_ptr<Type> base);
    bool IsArrayTy() const override;

    std::shared_ptr<Type> GetBase() override;
private:
    std::shared_ptr<Type> _base;
};

#endif