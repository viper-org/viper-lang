#include "parser/ast/statement/ConstexprStatement.h"

namespace parser
{
    ConstexprStatement::ConstexprStatement(Type* type, std::string&& name, ASTNodePtr&& value)
        : mName(std::move(name))
        , mValue(std::move(value))
    {
        mType = type;
    }

    vipir::Value* ConstexprStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* value = mValue->emit(builder, module, scope, diag);

        scope->locals[mName].alloca = value;

        return nullptr;
    }

}