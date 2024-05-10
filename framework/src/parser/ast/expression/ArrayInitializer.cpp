// Copyright 2024 solar-mist


#include "parser/ast/expression/ArrayInitializer.h"

#include "type/ArrayType.h"

#include <vipir/IR/Constant/ConstantArray.h>

namespace parser
{
    ArrayInitializer::ArrayInitializer(std::vector<ASTNodePtr>&& body)
        : mBody(std::move(body))
    {
        mType = ArrayType::Create(mBody[0]->getType(), mBody.size());
    }

    vipir::Value* ArrayInitializer::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        std::vector<vipir::Value*> values;
        for (auto& value : mBody)
        {
            values.push_back(value->emit(builder, module, scope, diag));
        }
        return vipir::ConstantArray::Get(module, mType->getVipirType(), std::move(values));
    }
}