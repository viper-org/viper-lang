#ifndef VIPER_PARSING_ASTNODE_HXX
#define VIPER_PARSING_ASTNODE_HXX
#include <environment.hxx>
#include <types/types.hxx>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <ostream>

namespace Viper
{
    namespace Parsing
    {
        enum class ASTNodeType
        {
            Integer,

            Variable,

            BinaryExpression,

            Return,

            VariableDeclaration,

            IfStatement
        };

        class ASTNode
        {
        public:
            virtual ~ASTNode() {  }

            virtual void Print(std::ostream& stream) const = 0;

            virtual llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope) = 0;

            ASTNodeType GetNodeType() const { return _nodeType; }
            std::shared_ptr<Type> GetType() const { return _type; }
            void SetType(std::shared_ptr<Type> type) { _type = type; }
        protected:
            ASTNodeType _nodeType;
            std::shared_ptr<Type> _type;
        };
    }
}

#endif