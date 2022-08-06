#ifndef QRK_AST_EXTERN_FUNC_HH
#define QRK_AST_EXTERN_FUNC_HH
#include <ast/expr.hh>

class extern_func
{
    std::string name;
    type_info type;
    std::vector<std::pair<type_info, std::string>> args;
public:
    extern_func(std::string, type_info, std::vector<std::pair<type_info, std::string>>);

    llvm::Function* codegen() const;
};

#endif