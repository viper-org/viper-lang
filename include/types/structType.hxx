#ifndef VIPER_STRUCT_TYPE_HXX
#define VIPER_STRUCT_TYPE_HXX
#include <types/type.hxx>

namespace Viper
{
    class StructType : public Type
    {
    public:
        StructType(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>> fields, unsigned int size, llvm::LLVMContext& context);

        llvm::Type* GetLLVMType(llvm::LLVMContext& context) const;

        unsigned int GetMemberIndex(std::string member);
    private:
        std::string _name;
        llvm::StructType* _llvmType;
        std::vector<std::pair<std::shared_ptr<Type>, std::string>> _fields;
    };
}

#endif