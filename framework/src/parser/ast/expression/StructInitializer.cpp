// Copyright 2024 solar-mist


#include "parser/ast/expression/StructInitializer.h"

#include <vipir/IR/Constant/ConstantStruct.h>

namespace parser
{
    StructInitializer::StructInitializer(Type* type, std::vector<ASTNodePtr>&& body, lexing::Token typeToken)
        : mBody(std::move(body))
        , mTypeToken(std::move(typeToken))
    {
        mType = type;
        mPreferredDebugToken = mTypeToken;
    }

    void StructInitializer::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mType->isStructType())
        {
            diag.compilerError(mTypeToken.getStart(), mTypeToken.getEnd(), std::format("Struct initializer cannot have type '{}{}{}'",
                fmt::bold, mType->getName(), fmt::defaults));
        }

        StructType* structType = static_cast<StructType*>(mType);
        
        int index = 0;
        for (auto& node : mBody)
        {
            if (node->getType() != structType->getFields()[index].type)
            {
                diag.compilerError(mTypeToken.getStart(), mTypeToken.getEnd(), std::format("Struct initializer field of type '{}{}{}' cannot have type '{}{}{}'",
                    fmt::bold, structType->getFields()[index].type->getName(), fmt::defaults,
                    fmt::bold, node->getType()->getName(), fmt::defaults));
            }
            ++index;
            node->typeCheck(scope, diag);
        }
    }

    vipir::Value* StructInitializer::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        std::vector<vipir::Value*> values;
        for (auto& value : mBody)
        {
            values.push_back(value->emit(builder, module, scope, diag));
        }
        return vipir::ConstantStruct::Get(module, mType->getVipirType(), std::move(values));
    }
}