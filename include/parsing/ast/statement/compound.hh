#ifndef VIPER_AST_STATEMENT_COMPOUND_HH
#define VIPER_AST_STATEMENT_COMPOUND_HH
#include <parsing/ast/astNode.hh>
#include <memory>
#include <vector>

namespace Parsing
{
    class CompoundStatement : public ASTNode
    {
    public:
        CompoundStatement(std::vector<std::unique_ptr<ASTNode>>& statements);

        void Print(std::ostream& stream, int indent) const override;

        SSA::Value* Emit(SSA::Builder& builder) override;
    private:
        std::vector<std::unique_ptr<ASTNode>> _statements;
    };
}

#endif