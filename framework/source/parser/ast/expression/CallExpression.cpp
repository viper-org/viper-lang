// Copyright 2024 solar-mist

#include "parser/ast/expression/CallExpression.h"
#include "parser/ast/expression/VariableExpression.h"

#include "type/FunctionType.h"

#include <vipir/Module.h>

#include <vipir/IR/Function.h>
#include <vipir/IR/Instruction/CallInst.h>

#include <algorithm>

namespace parser
{
    CallExpression::CallExpression(Scope* scope, ASTNodePtr callee, std::vector<ASTNodePtr> parameters)
        : ASTNode(scope, callee->getErrorToken())
        , mCallee(std::move(callee))
        , mParameters(std::move(parameters))
    {
    }

    vipir::Value* CallExpression::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        auto callee = mBestViableFunction->value;
        std::vector<vipir::Value*> parameters;
        for (auto& parameter : mParameters)
        {
            parameters.push_back(parameter->codegen(builder, module, diag));
        }

        return builder.CreateCall(static_cast<vipir::Function*>(callee), std::move(parameters));
    }

    void CallExpression::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        mCallee->typeCheck(diag, exit);
        for (auto& parameter : mParameters)
        {
            parameter->typeCheck(diag, exit);
        }
        mBestViableFunction = getBestViableFunction(diag);

        if (!mBestViableFunction)
        {
            exit = true; // Error will have been reported in getBestViableFunction
            mType = Type::Get("error-type");
        }
        else
        {
            auto functionType = static_cast<FunctionType*>(mBestViableFunction->type);
            mType = functionType->getReturnType();
            unsigned int index = 0;
            for (auto& parameter : mParameters)
            {
                auto argumentType = functionType->getArgumentTypes()[index];
                if (parameter->getType() != argumentType)
                {
                    if (parameter->implicitCast(diag, argumentType))
                    {
                        parameter = Cast(parameter, argumentType);
                    }
                    else
                    {
                        diag.reportCompilerError(
                            mErrorToken.getStartLocation(),
                            mErrorToken.getEndLocation(),
                            std::format("no matching function for call to '{}{}(){}'",
                                fmt::bold, mBestViableFunction->name, fmt::defaults)
                        );
                        exit = true;
                        mType = Type::Get("error-type");
                    }
                }
            }
        }
    }

    bool CallExpression::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }

    struct ViableFunction
    {
        Symbol* symbol;
        int score;
    };

    Symbol* CallExpression::getBestViableFunction(diagnostic::Diagnostics& diag)
    {
        if (auto var = dynamic_cast<VariableExpression*>(mCallee.get()))
        {
            auto candidateFunctions = mScope->getCandidateFunctions(var->getName());
            
            // Find all viable functions
            for (auto it = candidateFunctions.begin(); it != candidateFunctions.end();)
            {
                auto candidate = *it;
                if (!candidate->type->isFunctionType()) it = candidateFunctions.erase(it);
                else
                {
                    auto functionType = static_cast<FunctionType*>(candidate->type);
                    auto arguments = functionType->getArgumentTypes();
                    if (arguments.size() != mParameters.size()) it = candidateFunctions.erase(it);
                    else ++it;
                }
            }

            if (candidateFunctions.empty())
            {
                diag.reportCompilerError(
                    mErrorToken.getStartLocation(),
                    mErrorToken.getEndLocation(),
                    std::format("no matching function for call to '{}{}(){}'",
                        fmt::bold, var->getName(), fmt::defaults)
                );
                return nullptr;
            }

            std::vector<ViableFunction> viableFunctions;
            for (auto& candidate : candidateFunctions)
            {
                auto functionType = static_cast<FunctionType*>(candidate->type);
                int score = 0;
                for (size_t i = 0; i < mParameters.size(); ++i)
                {
                    auto castLevel = mParameters[i]->getType()->castTo(functionType->getArgumentTypes()[i]);
                    int multiplier = 0;
                    if (mParameters[i]->getType() == functionType->getArgumentTypes()[i]) multiplier = 0;
                    else if (castLevel == Type::CastLevel::Implicit) multiplier = 1;
                    else if (castLevel == Type::CastLevel::ImplicitWarning) multiplier = 2;
                    else multiplier = 10000;
                    score += multiplier * (mParameters.size() - i); // Weight earlier scores more
                }
                viableFunctions.push_back({candidate, score});
            }
            std::sort(viableFunctions.begin(), viableFunctions.end(), [](const auto& lhs, const auto& rhs){
                return lhs.score < rhs.score;
            });
            if (viableFunctions.size() >= 2)
            {
                if (viableFunctions[0].score == viableFunctions[1].score)
                {
                    diag.reportCompilerError(
                        mErrorToken.getStartLocation(),
                        mErrorToken.getEndLocation(),
                        std::format("call to '{}{}(){}' is ambiguous",
                            fmt::bold, var->getName(), fmt::defaults)
                    );
                    return nullptr;
                }
            }
            return viableFunctions.front().symbol;
        }
        return nullptr;
    }
}