#ifndef VIPER_PARSING_ASTNODE_HXX
#define VIPER_PARSING_ASTNODE_HXX
#include <environment.hxx>
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

            BinaryExpression,

            Return,
        };

        class ASTNode
        {
        public:
            virtual ~ASTNode() {  }

            virtual void Print(std::ostream& stream) const = 0;

            virtual llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope) = 0;

            ASTNodeType GetType() const { return _type; }
        protected:
            ASTNodeType _type;
        };
    }
}

#endif