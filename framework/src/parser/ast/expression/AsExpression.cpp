// Copyright 2023 solar-mist


#include "parser/ast/expression/AsExpression.h"
#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>
#include <vipir/IR/Instruction/SExtInst.h>

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
        
        if (mDestination->isIntegerType() && mSource->getType()->isIntegerType())
        {
            std::cout << "a\n";
            if (mDestination->getSize() > mSource->getType()->getSize()) // TODO: Add truncation
            {
                vipir::Value* source = mSource->emit(builder, module, scope);

                return builder.CreateSExt(source, mDestination->getVipirType());
            }
        }

        std::cerr << "Unimplemented cast.\n";
        std::exit(1);
    }
}