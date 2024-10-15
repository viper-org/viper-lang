// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/Instruction/AllocaInst.h>

namespace parser
{
    FunctionArgument::FunctionArgument(Type* type, std::string name)
        : type(type)
        , name(std::move(name))
    {
    }
    
    
    Function::Function(std::string name, FunctionType* type, std::vector<FunctionArgument> arguments, std::vector<ASTNodePtr> body, ScopePtr scope, lexer::Token token)
        : ASTNode(scope->parent, type, token)
        , mName(std::move(name))
        , mArguments(std::move(arguments))
        , mBody(std::move(body))
        , mOwnScope(std::move(scope))
    {
        mScope->symbols.emplace_back(mName, mType);
        for (auto& argument : mArguments)
        {
            mOwnScope->symbols.emplace_back(argument.name, argument.type);
        }
    }

    vipir::Value* Function::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        auto functionType = static_cast<vipir::FunctionType*>(mType->getVipirType());
        auto function = vipir::Function::Create(functionType, module, mName);

        auto entryBB = vipir::BasicBlock::Create("", function);
        builder.setInsertPoint(entryBB);

        mScope->resolveSymbol(mName)->value = function;

        unsigned int index = 0;
        for (auto& argument : mArguments)
        {
            auto alloca = builder.CreateAlloca(argument.type->getVipirType());
            auto arg = function->getArgument(index);
            builder.CreateStore(alloca, arg);

            mOwnScope->resolveSymbol(argument.name)->value = alloca;
        }

        for (auto& node : mBody)
        {
            node->codegen(builder, module, diag);
        }

        return nullptr;
    }
    
    void Function::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        for (auto& node : mBody)
        {
            node->typeCheck(diag, exit);
        }
    }

    bool Function::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }
}