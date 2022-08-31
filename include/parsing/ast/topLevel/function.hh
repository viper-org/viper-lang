#ifndef VIPER_AST_FUNCTION_HH
#define VIPER_AST_FUNCTION_HH
#include <parsing/ast/topLevel.hh>
#include <parsing/ast/astNode.hh>
#include <string>
#include <memory>
#include <vector>

class ASTFunction : public ASTTopLevel
{
public:
    ASTFunction(std::string name, std::vector<std::string> args, std::vector<std::unique_ptr<ASTNode>> body, Type* returnType);

    void Print(std::ostream& stream) const override;

    Codegen::Global* Generate(Codegen::Module& module, Codegen::Builder& builder) override;
private:
    std::string _name;
    std::vector<std::string> _args;
    std::vector<std::unique_ptr<ASTNode>> _body;
    Type* _returnType;
};

#endif