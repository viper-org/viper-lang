#ifndef VIPER_AST_WHILE_HH
#define VIPER_AST_WHILE_HH
#include <parsing/ast/astNode.hh>

namespace Parsing
{
    class WhileStatement : public ASTNode
    {
    public:
        WhileStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> body);

        void Print(std::ostream& stream, int indent) const override;

        SSA::Value* Emit(SSA::Builder& builder, Environment* scope, bool isStatement) override;
    private:
        std::unique_ptr<ASTNode> _cond;
        std::unique_ptr<ASTNode> _body;
    };
}

#endif