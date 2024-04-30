// Copyright 2024 solar-mist


#include "parser/ast/global/GlobalDeclaration.h"

#include "scope/Scope.h"

#include <vipir/Module.h>

namespace parser
{
    GlobalDeclaration::GlobalDeclaration(std::string name, Type* type, ASTNodePtr initVal)
        : mName(std::move(name))
        , mInitVal(std::move(initVal))
    {
        mType = type;
    }

    vipir::Value* GlobalDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::GlobalVar* global = module.createGlobalVar(mType->getVipirType());

        vipir::Value* initVal = mInitVal->emit(builder, module, scope);
        global->setInitialValue(initVal);

        GlobalVariables[mName] = global;

        return nullptr;
    }
}