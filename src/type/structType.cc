#include <type/structType.hh>
StructType::StructType(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>>& fields, llvm::LLVMContext& ctx)
    :Type(nullptr), 
    _name(name), _fields(fields)
{
    std::vector<llvm::Type*> fieldTypes;
    for(auto field : _fields)
        fieldTypes.push_back(field.first->GetLLVMType());
    llvm::StructType* structType = llvm::StructType::create(ctx, _name);
    structType->setBody(fieldTypes);
    _llvmType = structType;
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