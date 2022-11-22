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
    Type(int size, llvm::Type* llvmType)
        :_size(size), _llvmType(llvmType)
    {
    }
    virtual ~Type() {  }

    llvm::Type* GetLLVMType() const { return _llvmType; }

    int         GetScalarSize()    const { return _size; }

    virtual int GetPrimitiveSize() const { return _size; }

    virtual std::shared_ptr<Type> GetBase() { return std::shared_ptr<Type>(this); }

    static llvm::Value* Convert(llvm::Value* src, llvm::Type* dst, llvm::IRBuilder<>& builder);
    
    virtual bool IsIntegerTy() const { return false; }
    virtual bool IsPointerTy() const { return false; }
    virtual bool IsVoidTy()    const { return false; }
    virtual bool IsArrayTy()   const { return false; }
protected:
    int _size;
    llvm::Type* _llvmType;
};

#endif