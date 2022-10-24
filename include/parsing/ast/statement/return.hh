#ifndef VIPER_AST_STATEMENT_RETURN_HH
#define VIPER_AST_STATEMENT_RETURN_HH
#include <parsing/ast/astNode.hh>
#include <memory>

namespace Parsing
{
    class ReturnStatement : public ASTNode
    {
    public:
        ReturnStatement(std::unique_ptr<ASTNode> value, std::shared_ptr<Type> returnType);

        void Print(std::ostream& stream, int indent) const override;

        std::shared_ptr<Type> GetReturnType() const;

        SSA::Value* Emit(SSA::Builder& builder, bool isStatement) override;
    private:
        std::unique_ptr<ASTNode> _value;
        std::shared_ptr<Type> _returnType;
    };
}

#endif