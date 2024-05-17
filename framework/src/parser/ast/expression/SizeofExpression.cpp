#include "parser/ast/expression/SizeofExpression.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parser
{
    SizeofExpression::SizeofExpression(Type* expressionType, Type* type)
        : mTypeToSize(type)
    {
        mType = expressionType ? expressionType : Type::Get("i32");
    }

    vipir::Value* SizeofExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return vipir::ConstantInt::Get(module, mTypeToSize->getSize() / 8, mType->getVipirType());
    }
}