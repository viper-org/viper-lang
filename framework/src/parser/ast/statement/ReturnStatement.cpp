// Copyright 2023 solar-mist


#include "parser/ast/statement/ReturnStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

namespace parsing
{
    ReturnStatement::ReturnStatement()
    {
    }

    vipir::Value* ReturnStatement::emit(vipir::Builder& builder, vipir::Module&)
    {
        return builder.CreateRet(nullptr);
    }
}