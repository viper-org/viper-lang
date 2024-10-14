// Copyright 2024 solar-mist

#include "parser/ast/statement/VariableDeclaration.h"

#include <vipir/IR/Instruction/AllocaInst.h>

#include <algorithm>

namespace parser
{
    VariableDeclaration::VariableDeclaration(Scope* scope, std::string name, Type* type, ASTNodePtr initValue, lexer::Token token)
        : ASTNode(scope, type, token)
        , mName(std::move(name))
        , mInitValue(std::move(initValue))
    {
        mScope->symbols.emplace_back(mName, mType);
    }

    vipir::Value* VariableDeclaration::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        vipir::AllocaInst* alloca = builder.CreateAlloca(mType->getVipirType());

        if (mInitValue)
        {
            vipir::Value* initValue = mInitValue->codegen(builder, module, diag);
            builder.CreateStore(alloca, initValue);
        }

        auto it = std::find_if(mScope->symbols.begin(), mScope->symbols.end(), [this](const auto& symbol){
            return symbol.name == mName;
        });
        it->value = alloca;

        return nullptr;
    }
    
    void VariableDeclaration::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        if (mInitValue)
        {
            mInitValue->typeCheck(diag, exit);

            if (mInitValue->getType() != mType)
            {
                if (mInitValue->implicitCast(diag, mType))
                {
                    mInitValue = Cast(mInitValue, mType);
                }
                else
                {
                    diag.reportCompilerError(
                        mInitValue->getErrorToken().getStartLocation(),
                        mInitValue->getErrorToken().getEndLocation(),
                        std::format("value of type '{}{}{}' is not compatible with variable of type '{}{}{}'",
                            fmt::bold, mInitValue->getType()->getName(), fmt::defaults,
                            fmt::bold, mType->getName(), fmt::defaults)
                    );
                    exit = true;
                }
            }
        }
    }

    bool VariableDeclaration::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }
}