#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
#include <ostream>

namespace Parsing
{
    enum class ASTNodeType
    {
        Integer,

        ReturnStatement,
        
        VariableDeclaration,
        Function,
    };

    class ASTNode
    {
    friend class ASTFunction;
    public:
        virtual ~ASTNode() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;

        ASTNodeType GetNodeType() const { return _nodeType; }
    protected:
        ASTNodeType _nodeType;
    };
}

#endif