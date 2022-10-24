#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
#include <ssa/ssa.hh>
#include <type/types.hh>
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
        Call,
    };

    class ASTNode
    {
    public:
        ASTNode(ASTNodeType type) :_nodeType(type) {  }
        virtual ~ASTNode() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;

        ASTNodeType GetNodeType() const { return _nodeType; }
        std::shared_ptr<Type> GetType() const { return _type; }

        virtual SSA::Value* Emit(SSA::Builder& builder, bool isStatement = false) = 0;
    protected:
        ASTNodeType _nodeType;
        std::shared_ptr<Type> _type;
    };
}

#endif