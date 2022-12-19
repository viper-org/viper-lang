#ifndef VIPER_STRUCT_TYPE_HXX
#define VIPER_STRUCT_TYPE_HXX
#include <type/type.hh>

class StructType : public Type
{
public:
    static std::shared_ptr<StructType> Create(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>>& fields, llvm::LLVMContext& ctx);

    bool IsStructTy() const override;

    void SetBody(const std::vector<std::pair<std::shared_ptr<Type>, std::string>>& fields);
    static std::string EmitStructSymbols();

    static std::shared_ptr<StructType> FindStructType(std::string_view name);

    std::pair<unsigned int, llvm::Type*> GetMemberIndex(std::string member);

    StructType(const std::string name, const std::vector<std::pair<std::shared_ptr<Type>, std::string>>& fields, llvm::LLVMContext& ctx);
private:
    std::string _name;
    std::vector<std::pair<std::shared_ptr<Type>, std::string>> _fields;
    
    std::string EmitSymbol();
};

#endif