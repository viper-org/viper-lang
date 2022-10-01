#ifndef VIPER_AST_STATEMENT_RETURN_HH
#define VIPER_AST_STATEMENT_RETURN_HH
#include <parsing/ast/astNode.hh>
#include <memory>

namespace Parsing
{
    class ReturnStatement : public ASTNode
    {
    public:
        ReturnStatement(std::unique_ptr<ASTNode> value);

        void Print(std::ostream& stream, int indent) const override;
    private:
        std::unique_ptr<ASTNode> _value;
    };
}

#endif