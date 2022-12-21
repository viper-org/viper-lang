#ifndef VIPER_TYPE_HH
#define VIPER_TYPE_HH
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <string>

class Type
{
public:
    Type(llvm::Type* llvmType)
        :_llvmType(llvmType)
    {
    }

    virtual ~Type() {  }

    llvm::Type* GetLLVMType() const { return _llvmType; }

    virtual std::shared_ptr<Type> GetBase() { return std::shared_ptr<Type>(this); }

    static std::shared_ptr<Type> FromString(std::string_view str);
    
    virtual bool IsIntegerTy() const { return false; }
    virtual bool IsPointerTy() const { return false; }
    virtual bool IsVoidTy()    const { return false; }
    virtual bool IsArrayTy()   const { return false; }
    virtual bool IsStructTy()  const { return false; }

    std::string GetMangleID();
protected:
    llvm::Type* _llvmType;
};

#endif