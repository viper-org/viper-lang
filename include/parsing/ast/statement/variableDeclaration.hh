#ifndef VIPER_AST_STATEMENT_VARIABLE_DELCARATION_HH
#define VIPER_AST_STATEMENT_VARIABLE_DELCARATION_HH
#include <parsing/ast/astNode.hh>
#include <string>
#include <optional>
#include <vector>
#include <memory>

namespace Parsing
{
    class VariableDeclaration : public ASTNode
    {
    public:
        VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, bool isFunction);

        void Print(std::ostream& stream, int indent) const override;

        bool IsFunction() const;
    private:
        std::string _name;
        std::unique_ptr<ASTNode> _initVal;
        bool _isFunction;
    };
}

#endif