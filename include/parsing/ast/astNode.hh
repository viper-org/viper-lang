#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <ostream>

namespace Parsing
{
    enum class ASTNodeType
    {
        Integer,

        ReturnStatement,

        BinaryExpression,
        
        VariableDeclaration,
        Function,

        CompoundStatement,
    };

    class ASTNode
    {
    public:
        ASTNode(ASTNodeType type) :_nodeType(type) {  }
        virtual ~ASTNode() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;

        ASTNodeType GetNodeType() const { return _nodeType; }

        virtual llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder) = 0;
    protected:
        ASTNodeType _nodeType;
    };
}

#endif