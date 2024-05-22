// Copyright 2024 solar-mist


#include "parser/ast/expression/ArrayInitializer.h"

#include "type/ArrayType.h"

#include <vipir/IR/Constant/ConstantArray.h>

namespace parser
{
    ArrayInitializer::ArrayInitializer(std::vector<ASTNodePtr>&& body, lexing::Token token)
        : mBody(std::move(body))
    {
        mType = ArrayType::Create(mBody[0]->getType(), mBody.size());
        mPreferredDebugToken = std::move(token);
    }

    void ArrayInitializer::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mType->isArrayType())
        {
            diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("Array initializer cannot have type '{}{}{}'",
                fmt::bold, mType->getName(), fmt::defaults));
        }

        Type* elementType = static_cast<ArrayType*>(mType)->getBaseType();
        for (auto& node : mBody)
        {
            if (node->getType() != elementType)
            {
                diag.compilerError(node->getDebugToken().getStart(), node->getDebugToken().getEnd(), "Array initializer values must have the same type");
            }
            node->typeCheck(scope, diag);
        }
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