#ifndef VIPER_TYPE_HH
#define VIPER_TYPE_HH
#include <memory>
#include <string>

class Type
{
public:
    Type(int size)
        :_size(size)
    {
    }
    virtual ~Type() {  }

    int         GetScalarSize()    const { return _size; }

    virtual int GetPrimitiveSize() const { return _size; }

    virtual std::shared_ptr<Type> GetBase() { return std::shared_ptr<Type>(this); }

    
    virtual bool IsIntegerTy() const { return false; }
    virtual bool IsPointerTy() const { return false; }
    virtual bool IsVoidTy()    const { return false; }
protected:
    int _size;
};

#endif