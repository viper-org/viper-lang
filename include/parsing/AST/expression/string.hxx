#ifndef VIPER_PARSING_AST_STRING_HXX
#define VIPER_PARSING_AST_STRING_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class StringLiteral : public ASTNode
        {
        public:
            StringLiteral(std::string text);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags = {}) override;
        private:
            std::string _text;
        };
    }
}

#endif