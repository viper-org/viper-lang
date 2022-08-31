#ifndef VIPER_AST_RETURN_HH
#define VIPER_AST_RETURN_HH
#include <parsing/ast/astNode.hh>
#include <memory>

class ReturnStatement : public ASTNode
{
public:
    ReturnStatement(std::unique_ptr<ASTNode> value);

    void Print(std::ostream& stream, int indent) const override;

    Codegen::Value* Generate(Codegen::Module& module, Codegen::Builder& builder, bool isStatement = false) override;
private:
    std::unique_ptr<ASTNode> _value;
};

#endif