#ifndef QRK_AST_EXTERN_FUNC_HH
#define QRK_AST_EXTERN_FUNC_HH
#include <ast/expr.hh>
#include <globals.hh>
#include <typing/types.hh>
#include <memory>
#include <vector>

class extern_func
{
    std::string name;
    std::shared_ptr<quark_type> type;
    std::vector<std::pair<std::shared_ptr<quark_type>, std::string>> args;
public:
    extern_func(std::string, std::shared_ptr<quark_type>, std::vector<std::pair<std::shared_ptr<quark_type>, std::string>>);

    llvm::Function* codegen() const;
};

#endif