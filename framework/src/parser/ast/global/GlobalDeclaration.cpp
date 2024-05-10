// Copyright 2024 solar-mist


#include "parser/ast/global/GlobalDeclaration.h"

#include <vipir/Module.h>

namespace parser
{
    GlobalDeclaration::GlobalDeclaration(std::string name, Type* type, ASTNodePtr initVal)
        : mName(std::move(name))
        , mInitVal(std::move(initVal))
    {
        mType = type;
    }

    vipir::Value* GlobalDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::GlobalVar* global = module.createGlobalVar(mType->getVipirType());

        vipir::Value* initVal = mInitVal->emit(builder, module, scope, diag);
        global->setInitialValue(initVal);

        GlobalVariables[mName] = global;

        return nullptr;
    }
}