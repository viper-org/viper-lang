#ifndef VIPER_STRUCT_TYPE_HXX
#define VIPER_STRUCT_TYPE_HXX
#include <type/type.hh>

class StructType : public Type
{
public:
    StructType(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>>& fields, llvm::LLVMContext& context);

    unsigned int GetMemberIndex(std::string member);
private:
    std::string _name;
    std::vector<std::pair<std::shared_ptr<Type>, std::string>> _fields;
};

#endif