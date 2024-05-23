// Copyright 2024 solar-mist


#include "parser/ast/global/GlobalDeclaration.h"

#include "symbol/Identifier.h"

#include <vipir/Module.h>

namespace parser
{
    GlobalDeclaration::GlobalDeclaration(std::vector<std::string> names, Type* type, ASTNodePtr initVal)
        : mNames(std::move(names))
        , mInitVal(std::move(initVal))
    {
        mType = type;

        std::string mangledName = "_G" + mType->getMangleID();
        for (auto& name : mNames)
        {
            mangledName += std::to_string(name.length());
            mangledName += name;
        }
        symbol::AddIdentifier(mangledName, mNames);
    }

    void GlobalDeclaration::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (mInitVal->getType() != mType)
        {
            diag.compilerError(mInitVal->getDebugToken().getStart(), mInitVal->getDebugToken().getEnd(), std::format("Global variable of type '{}{}{}' cannot be initialized with a value of type '{}{}{}",
                fmt::bold, mType->getName(), fmt::defaults,
                fmt::bold, mInitVal->getType()->getName(), fmt::defaults));
        }
        mInitVal->typeCheck(scope, diag);
    }

    vipir::Value* GlobalDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        std::string mangledName = "_G" + mType->getMangleID();
        for (auto& name : mNames)
        {
            mangledName += std::to_string(name.length());
            mangledName += name;
        }

        vipir::GlobalVar* global = module.createGlobalVar(mType->getVipirType());

        vipir::Value* initVal = mInitVal->emit(builder, module, scope, diag);
        global->setInitialValue(initVal);

        GlobalVariables[mangledName] = GlobalSymbol(global, mType);

        return nullptr;
    }
}