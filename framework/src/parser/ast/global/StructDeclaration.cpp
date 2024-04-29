// Copyright 2024 solar-mist


#include "parser/ast/global/StructDeclaration.h"
#include "type/StructType.h"

namespace parser
{
    StructDeclaration::StructDeclaration(std::string name, std::vector<StructField> fields)
        : mName(std::move(name))
        , mFields(std::move(fields))
    {
        std::vector<StructType::Field> fieldTypes;
        for (auto field : mFields)
        {
            fieldTypes.push_back({field.name, field.type});
        }
        StructType::Create(mName, std::move(fieldTypes));
    }

    vipir::Value* StructDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        return nullptr;
    }
}