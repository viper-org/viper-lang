#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
#include <codegen/value/value.hh>
#include <codegen/module.hh>
#include <codegen/builder.hh>
#include <ostream>

enum class ASTNodeType
{
    Integer,

    Variable,
    VariableDeclaration,

    BinaryExpression,

    ReturnStatement,
};

class ASTNode
{
friend class ASTFunction;
public:
    virtual ~ASTNode() {  }

    virtual void Print(std::ostream& stream, int indent) const = 0;

    ASTNodeType GetNodeType() const { return _nodeType; }
    Type* GetType() const { return _type; }

    virtual Codegen::Value* Generate(Codegen::Module& module, Codegen::Builder& builder, bool isStatement = false) = 0;
protected:
    ASTNodeType _nodeType;
    Type* _type;
};

#endif