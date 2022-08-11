#ifndef VIPER_PARSING_AST_VARIABLE_HXX
#define VIPER_PARSING_AST_VARIABLE_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class Variable : public ASTNode
        {
        public:
            Variable(std::string name);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope) override;
        private:
            std::string _name;
        };
    }
}

#endif