// Copyright 2024 solar-mist


#include "parser/ast/global/StructDeclaration.h"

#include "type/StructType.h"
#include "type/PointerType.h"

#include "symbol/NameMangling.h"
#include "symbol/Identifier.h"

#include <vipir/IR/BasicBlock.h>
#include <vipir/Type/FunctionType.h>

#include <vector>

namespace parser
{
    StructDeclaration::StructDeclaration(std::vector<std::string> names, std::vector<StructField> fields, std::vector<StructMethod> methods, Type* type)
        : mNames(std::move(names))
        , mFields(std::move(fields))
        , mMethods(std::move(methods))
    {
        mType = type;
        symbol::AddIdentifier(mType->getMangleID(), mNames);
    }

    std::vector<std::string>& StructDeclaration::getNames()
    {
        return mNames;
    }

    std::vector<StructField>& StructDeclaration::getFields()
    {
        return mFields;
    }

    std::vector<StructMethod>& StructDeclaration::getMethods()
    {
        return mMethods;
    }

    vipir::Value* StructDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        for (StructMethod& method : mMethods)
        {
            std::vector<Type*> manglingArguments;
            std::vector<vipir::Type*> argumentTypes;

            manglingArguments.push_back(PointerType::Create(mType));
            argumentTypes.push_back(vipir::Type::GetPointerType(mType->getVipirType()));

            for (auto& argument : method.arguments)
            {
                manglingArguments.push_back(argument.type);
                argumentTypes.push_back(argument.type->getVipirType());
            }

            vipir::FunctionType* functionType = vipir::FunctionType::Create(method.returnType->getVipirType(), argumentTypes);

            std::vector<std::string> names = mNames;
            names.push_back(method.name);
            std::string name = symbol::mangleFunctionName(names, std::move(manglingArguments));

            vipir::Function* func = vipir::Function::Create(functionType, module, name);
            FunctionSymbol::Create(func, name, names, method.priv);

            if (method.body.empty())
            {
                continue;
            }

            scope = method.scope.get();

            vipir::BasicBlock* entryBasicBlock = vipir::BasicBlock::Create("", func);
            builder.setInsertPoint(entryBasicBlock);

            int index = 0;

            vipir::AllocaInst* alloca = builder.CreateAlloca(vipir::Type::GetPointerType(mType->getVipirType()));
            scope->locals["this"].alloca = alloca;

            builder.CreateStore(alloca, func->getArgument(index++));

            for (auto& argument : method.arguments)
            {
                vipir::AllocaInst* alloca = builder.CreateAlloca(argument.type->getVipirType());
                scope->locals[argument.name].alloca = alloca;

                builder.CreateStore(alloca, func->getArgument(index++));
            }

            for (auto& node : method.body)
            {
                node->emit(builder, module, scope, diag);
            }
        }

        return nullptr;
    }
}