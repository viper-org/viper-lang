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