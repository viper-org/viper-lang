// Copyright 2024 solar-mist


#include "parser/ast/expression/StructInitializer.h"

#include <vipir/IR/Constant/ConstantStruct.h>

namespace parser
{
    StructInitializer::StructInitializer(Type* type, std::vector<ASTNodePtr>&& body)
        : mBody(std::move(body))
    {
        mType = type;
    }

    vipir::Value* StructInitializer::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        std::vector<vipir::Value*> values;
        for (auto& value : mBody)
        {
            values.push_back(value->emit(builder, module, scope));
        }
        return vipir::ConstantStruct::Get(module, mType->getVipirType(), std::move(values));
    }
}