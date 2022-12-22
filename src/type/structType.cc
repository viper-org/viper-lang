#include <type/structType.hh>

static std::vector<std::shared_ptr<StructType>> structTypes;

StructType::StructType(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>> fields, llvm::LLVMContext& ctx)
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

std::string StructType::EmitStructSymbols()
{
    std::string res;
    for(std::shared_ptr<StructType> type : structTypes)
        res += type->EmitSymbol();

    return res;
}

std::shared_ptr<StructType> StructType::Create(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>> fields, llvm::LLVMContext& ctx)
{
    std::shared_ptr<StructType> type = std::make_shared<StructType>(name, fields, ctx);

    structTypes.push_back(type);

    return type;
}

void StructType::ResetStructTypeArray()
{
    structTypes.clear();
}

bool StructType::IsStructTy() const
{
    return true;
}

void StructType::SetBody(const std::vector<std::pair<std::shared_ptr<Type>, std::string>>& fields)
{
    _fields = fields;
    std::vector<llvm::Type*> fieldTypes;
    for(auto field : _fields)
        fieldTypes.push_back(field.first->GetLLVMType());
    static_cast<llvm::StructType*>(_llvmType)->setBody(fieldTypes);
}

std::shared_ptr<StructType> StructType::FindStructType(std::string_view name)
{
    for(std::shared_ptr<StructType> type : structTypes)
    {
        if(type->_name == name)
            return type;
    }
    return nullptr;
}

std::pair<unsigned int, llvm::Type*> StructType::GetMemberIndex(std::string member)
{
    unsigned int i;
    for(i = 0; i < _fields.size(); i++)
    {
        if(_fields[i].second == member)
            return std::make_pair(i, _fields[i].first->GetLLVMType());
    }
    return std::make_pair(-1, nullptr); // TODO: Error properly
}

std::string StructType::EmitSymbol()
{
    std::string res = "#" + std::to_string(_name.length()) + _name + std::to_string(_fields.size());
    for(std::pair<std::shared_ptr<Type>, std::string> field : _fields)
    {
        res += field.first->GetMangleID();
        res += std::to_string(field.second.length());
        res += field.second;
    }

    return res;
}