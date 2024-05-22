#include "parser/ast/expression/SizeofExpression.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parser
{
    SizeofExpression::SizeofExpression(Type* expressionType, Type* type, lexing::Token token)
        : mTypeToSize(type)
    {
        mType = expressionType ? expressionType : Type::Get("i32");
        mPreferredDebugToken = std::move(token);
    }

    void SizeofExpression::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mType->isIntegerType())
        {
            diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("Sizeof expression cannot have type '{}{}{}'",
                fmt::bold, mType->getName(), fmt::defaults));
        }
    }

    vipir::Value* SizeofExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return vipir::ConstantInt::Get(module, mTypeToSize->getSize() / 8, mType->getVipirType());
    }
}