#include <types/structType.hxx>

namespace Viper
{
    StructType::StructType(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>> fields, unsigned int size, llvm::LLVMContext& context)
        :Type(name, name, size), _name(name), _fields(fields)
    {
        _llvmType = llvm::StructType::create(context, _name);
        std::vector<llvm::Type*> fieldTypes;
        for(auto field : _fields)
            fieldTypes.push_back(field.first->GetLLVMType(context));
        _llvmType->setBody(fieldTypes);
    }

    llvm::Type* StructType::GetLLVMType(llvm::LLVMContext&) const
    {
        return _llvmType;
    }

    unsigned int StructType::GetMemberIndex(std::string member)
    {
        unsigned int i;
        for(i = 0; i < _fields.size(); i++)
        {
            if(_fields[i].second == member)
                return i;
        }
        return -1; // TODO: Error properly
    }
}