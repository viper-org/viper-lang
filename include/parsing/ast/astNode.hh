#ifndef VIPER_AST_ASTNODE_HH
#define VIPER_AST_ASTNODE_HH
#include <environment.hh>
#include <type/types.hh>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <ostream>

namespace Parsing
{
    enum class ASTNodeType
    {
        Integer,
        String,

        ReturnStatement,
        IfStatement,

        BinaryExpression,
        
        VariableDeclaration,
        Variable,
        Function,
        Call,

        CompoundStatement,
    };

    class ASTNode
    {
    public:
        ASTNode(ASTNodeType type) :_nodeType(type) {  }
        virtual ~ASTNode() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;

        ASTNodeType GetNodeType() const { return _nodeType; }
        std::shared_ptr<Type> GetType() const { return _type; }

        virtual llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) = 0;
    protected:
        ASTNodeType _nodeType;
        std::shared_ptr<Type> _type;
    };
}

#endif