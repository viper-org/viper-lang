#ifndef VIPER_AST_VARIABLE_DECL_HH
#define VIPER_AST_VARIABLE_DECL_HH
#include <parsing/ast/astNode.hh>
#include <type/types.hh>
#include <memory>

class VariableDeclaration : public ASTNode
{
public:
    VariableDeclaration(Type* type, const std::string& name,std::unique_ptr<ASTNode> value);

    void Print(std::ostream& stream, int indent) const override;

    Codegen::Value* Generate(Codegen::Module& module, Codegen::Builder& builder, bool isStatement = false) override;
private:
    Type* _type;
    std::string _name;
    std::unique_ptr<ASTNode> _value;
};

#endif