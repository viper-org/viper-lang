#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
#include <ssa/ssa.hh>
#include <ostream>

namespace Parsing
{
    enum class ASTNodeType
    {
        Integer,
        BinaryExpression,

        Variable,

        ReturnStatement,
        CompoundStatement,
        
        VariableDeclaration,
        Function,
    };

    class ASTNode
    {
    public:
        virtual ~ASTNode() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;

        ASTNodeType GetNodeType() const { return _nodeType; }

        virtual SSA::Value* Emit(SSA::Builder& builder) = 0;
    protected:
        ASTNodeType _nodeType;
    };
}

#endif