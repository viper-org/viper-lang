#ifndef QRK_AST_FUNCTION_HH
#define QRK_AST_FUNCTION_HH
#include <ast/expr.hh>
#include <globals.hh>
#include <typing/types.hh>
#include <memory>
#include <vector>

class function_ast
{
    std::string name;
    std::shared_ptr<quark_type> type;
    std::vector<std::unique_ptr<ast_expr>> body;
    std::vector<std::pair<std::shared_ptr<quark_type>, std::string>> args;
    std::shared_ptr<scope> env;
public:
    function_ast(std::string, std::shared_ptr<quark_type>, std::vector<std::unique_ptr<ast_expr>>, std::vector<std::pair<std::shared_ptr<quark_type>, std::string>>, std::shared_ptr<scope>);
    
    llvm::Function* codegen();
};

#endif