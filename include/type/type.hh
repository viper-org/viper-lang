#ifndef VIPER_TYPE_HH
#define VIPER_TYPE_HH
#include <string>

namespace Codegen
{
    class Builder;
    class Value;
}
class Type
{
public:
    Type(const std::string& name, int size)
        :_name(name), _size(size)
    {
    }
    virtual ~Type() {  }

    std::string_view GetName()   const { return _name; }
    int              GetSize()   const { return _size; }
    std::string      GetSuffix() const { return _instructionSuffix; }

    static Codegen::Value* Convert(Codegen::Value* value, Type* type, Codegen::Builder& builder);

    
    virtual bool IsIntegerTy() const { return false; }
protected:
    std::string _instructionSuffix;
    std::string _name;
    int         _size;
};

#endif