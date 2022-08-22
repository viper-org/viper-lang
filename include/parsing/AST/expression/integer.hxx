#ifndef VIPER_PARSING_AST_INTEGER_HXX
#define VIPER_PARSING_AST_INTEGER_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class IntegerLiteral : public ASTNode
        {
        public:
            IntegerLiteral(int value);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        private:
            int _value;
        };
    }
}

#endif