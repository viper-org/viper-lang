#ifndef VIPER_AST_VARIABLE_HH
#define VIPER_AST_VARIABLE_HH
#include <parsing/ast/astNode.hh>

class Variable : public ASTNode
{
public:
    Variable(const std::string& name);

    void Print(std::ostream& stream, int indent) const override;

    Codegen::Value* Generate(Codegen::Module& module, Codegen::Builder& builder, bool isStatement = false) override;

    std::string GetName() const;
private:
    std::string _name;
};

#endif