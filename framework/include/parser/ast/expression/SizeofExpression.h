#ifndef VIPER_SIZEOFEXPRESSION_H
#define VIPER_SIZEOFEXPRESSION_H

#include "parser/ast/Node.h"

namespace parser
{
    class SizeofExpression : public ASTNode
    {
    public:
        SizeofExpression(Type* expressionType, Type* type, lexing::Token token);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        Type* mTypeToSize;
    };

    using SizeofExpressionPtr = std::unique_ptr<SizeofExpression>;
}

#endif //VIPER_SIZEOFEXPRESSION_H
