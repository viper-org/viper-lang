// Copyright 2023 solar-mist


#include "parser/ast/expression/AsExpression.h"
#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>

#include <iostream>

namespace parsing
{
    AsExpression::AsExpression(ASTNodePtr source, Type* destination)
        : mSource(std::move(source))
        , mDestination(destination)
    {
    }

    vipir::Value* AsExpression::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        if (IntegerLiteral* literal = dynamic_cast<IntegerLiteral*>(mSource.get()))
        {
            return builder.CreateConstantInt(literal->getValue(), mDestination->getVipirType());
        }
        std::cerr << "Unimplemented cast.\n";
        std::exit(1);
    }
}