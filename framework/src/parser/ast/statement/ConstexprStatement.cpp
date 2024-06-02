#include "parser/ast/statement/ConstexprStatement.h"
#include "symbol/Identifier.h"

namespace parser
{
    ConstexprStatement::ConstexprStatement(Type* type, std::vector<std::string> names, ASTNodePtr&& value, lexing::Token token, bool global)
        : mNames(std::move(names))
        , mValue(std::move(value))
        , mToken(std::move(token))
        , mGlobal(global)
    {
        mType = type;

        if (mGlobal)
        {
            std::string mangledName = "_CE" + mType->getMangleID();
            for (auto& name : mNames)
            {
                mangledName += std::to_string(name.length());
                mangledName += name;
            }
            symbol::AddIdentifier(mangledName, mNames);
            GlobalVariables[mangledName] = GlobalSymbol(nullptr, mType);
        }
    }

    void ConstexprStatement::typeCheck(Scope *scope, diagnostic::Diagnostics &diag)
    {
        if (mValue)
        {
            if (mValue->getType() != mType)
            {
                diag.compilerError(mValue->getDebugToken().getStart(), mValue->getDebugToken().getEnd(), std::format("Constexpr Variable of type '{}{}{}' cannot be initialized with a value of type '{}{}{}'",
                    fmt::bold, mType->getName(), fmt::defaults,
                    fmt::bold, mValue->getType()->getName(), fmt::defaults));
            }
            mValue->typeCheck(scope, diag);
        }
    }

    vipir::Value* ConstexprStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mValue) return nullptr;

        if (!mGlobal)
        {
            vipir::Value* constant = mValue->emit(builder, module, scope, diag);

            if (mNames.size() != 1)
                diag.compilerError(mToken.getStart(), mToken.getEnd(), "internal parser error");

            scope->locals[mNames[0]].alloca = constant;
        }
        else
        {
            std::string mangledName = "_CE" + mType->getMangleID();
            for (auto& name : mNames)
            {
                mangledName += std::to_string(name.length());
                mangledName += name;
            }

            vipir::Value* constant = mValue->emit(builder, module, scope, diag);
            GlobalVariables[mangledName] = GlobalSymbol(constant, mType);
        }

        return nullptr;
    }
}