#ifndef QRK_AST_FUNCTION_HH
#define QRK_AST_FUNCTION_HH
#include <ast/expr.hh>
#include <globals.hh>
#include <typing/type_info.hh>
#include <memory>
#include <vector>

class function_ast
{
    std::string name;
    type_info type;
    std::vector<std::unique_ptr<ast_expr>> body;
    std::vector<std::pair<type_info, std::string>> args;
    std::shared_ptr<scope> env;
public:
    function_ast(std::string, type_info, std::vector<std::unique_ptr<ast_expr>>, std::vector<std::pair<type_info, std::string>>, std::shared_ptr<scope>);
    
    llvm::Function* codegen();
};

#endif