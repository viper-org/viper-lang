#ifndef VIPER_AST_TOPLEVEL_FUNCTION_HXX
#define VIPER_AST_TOPLEVEL_FUNCTION_HXX
#include <parsing/AST/topLevel.hxx>
#include <parsing/AST/astNode.hxx>
#include <memory>
#include <vector>

namespace Viper
{
    namespace Parsing
    {
        class ASTFunction : public ASTTopLevel
        {
        public:
            ASTFunction(std::string name, std::vector<std::unique_ptr<ASTNode>> body);

            void Print(std::ostream& stream) const override;
        private:
            std::string _name;
            std::vector<std::unique_ptr<ASTNode>> _body;
        };
    }
}

#endif