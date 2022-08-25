#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
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
private:
    ASTNodeType _nodeType;
};

#endif