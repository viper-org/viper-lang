#ifndef VIPER_INTEGER_TYPE_HH
#define VIPER_INTEGER_TYPE_HH
#include <type/type.hh>

class IntegerType : public Type
{
public:
    IntegerType(int bits);
    bool IsIntegerTy() const override;
};

#endif