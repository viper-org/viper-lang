#ifndef VIPER_AST_VARIABLE_DECL_HH
#define VIPER_AST_VARIABLE_DECL_HH
#include <parsing/ast/astNode.hh>
#include <memory>

class VariableDeclaration : public ASTNode
{
public:
    VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> value);

    void Print(std::ostream& stream, int indent) const override;

    Codegen::Value* Generate(Codegen::Module& module, Codegen::Builder& builder) override;
private:
    std::string _name;
    std::unique_ptr<ASTNode> _value;
};

#endif