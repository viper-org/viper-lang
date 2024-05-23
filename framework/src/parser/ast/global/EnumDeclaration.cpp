// Copyright 2024 solar-mist


#include "parser/ast/global/EnumDeclaration.h"

#include "symbol/Identifier.h"

#include "type/EnumType.h"

#include <vipir/IR/Constant/ConstantInt.h>
#include <vipir/Module.h>

namespace parser
{
    EnumDeclaration::EnumDeclaration(std::vector<std::string> names, std::vector<EnumField> fields)
        : mNames(std::move(names))
        , mFields(std::move(fields))
    {
        mType = EnumType::Create(mNames);
        symbol::AddIdentifier(mType->getMangleID(), mNames);

        for (auto& field : mFields)
        {
            std::string mangledName = "_EM" + field.name;

            std::vector<std::string> names = mNames;
            names.push_back(field.name);

            symbol::AddIdentifier(std::move(mangledName), std::move(names));
        }
    }

    void EnumDeclaration::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
    }

    vipir::Value* EnumDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        for (auto& field : mFields)
        {
            std::string mangledName = "_EM" + field.name;

            vipir::Value* constant = vipir::ConstantInt::Get(module, field.value, vipir::Type::GetIntegerType(32));
            GlobalVariables[mangledName] = GlobalSymbol(constant, mType);
        }

        return nullptr;
    }
}