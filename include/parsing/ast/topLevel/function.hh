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
    ASTFunction(std::string name, std::vector<std::unique_ptr<ASTNode>> body);

    void Print(std::ostream& stream) const override;
private:
    std::string _name;
    std::vector<std::unique_ptr<ASTNode>> _body;
    // TODO: Add args
};

#endif