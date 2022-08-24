#ifndef VIPER_AST_TOPLEVEL_HXX
#define VIPER_AST_TOPLEVEL_HXX
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <codegen/symbol.hxx>
#include <ostream>

namespace Viper
{
    namespace Parsing
    {
        class ASTTopLevel
        {
        public:
            virtual ~ASTTopLevel() {  }

            virtual void Print(std::ostream& stream) const = 0;

            virtual std::pair<llvm::Value*, std::unique_ptr<CodeGen::Symbol>> Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module) = 0;
        };
    }
}

#endif