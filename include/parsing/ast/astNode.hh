#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
#include <codegen/value/value.hh>
#include <codegen/module.hh>
#include <codegen/builder.hh>
#include <ostream>

enum class ASTNodeType
{
    Integer,

    ReturnStatement
};

class ASTNode
{
public:
    virtual ~ASTNode() {  }

    virtual void Print(std::ostream& stream, int indent) const = 0;

    ASTNodeType GetNodeType() const { return _nodeType; }

    virtual Codegen::Value* Generate(Codegen::Module& module, Codegen::Builder& builder) = 0;
private:
    ASTNodeType _nodeType;
};

#endif