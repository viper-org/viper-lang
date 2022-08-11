#ifndef VIPER_TYPE_HXX
#define VIPER_TYPE_HXX
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <string>

namespace Viper
{
    class Type
    {
    public:
        Type(std::string name, std::string llvmName, int size)
            :_name(name), _llvmName(llvmName), _size(size)
        {
        }
        virtual ~Type() {  }

        std::string_view GetName()     const { return _name; }
        std::string_view GetLLVMName() const { return _llvmName; }
        int              GetSize()     const { return _size; }

        virtual llvm::Type* GetLLVMType(llvm::LLVMContext& context) const = 0;

        static llvm::Value* Convert(llvm::Value* value, llvm::Type* type, llvm::IRBuilder<>& builder);
    protected:
        std::string _name;
        std::string _llvmName;
        int         _size;
    };
}

#endif