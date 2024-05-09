// Copyright 2024 solar-mist


#include "parser/ast/global/StructDeclaration.h"

#include "type/StructType.h"

#include <vipir/IR/BasicBlock.h>
#include <vipir/Type/FunctionType.h>

#include <vector>

namespace parser
{
    StructDeclaration::StructDeclaration(std::string name, std::vector<StructField> fields, std::vector<StructMethod> methods)
        : mName(std::move(name))
        , mFields(std::move(fields))
        , mMethods(std::move(methods))
    {
        std::vector<StructType::Field> fieldTypes;
        for (auto& field : mFields)
        {
            fieldTypes.push_back({field.priv, field.name, field.type});
        }

        mType = StructType::Create(mName, std::move(fieldTypes));
    }

    vipir::Value* StructDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        for (StructMethod& method : mMethods)
        {
            scope = method.scope.get();

            std::vector<vipir::Type*> argumentTypes;
            argumentTypes.push_back(vipir::Type::GetPointerType(mType->getVipirType()));

            for (auto& argument : method.arguments)
            {
                argumentTypes.push_back(argument.type->getVipirType());
            }
            vipir::FunctionType* functionType = vipir::FunctionType::Create(method.returnType->getVipirType(), argumentTypes);
            std::string name = mangleMethodName(mName, method.name);

            vipir::Function* func = vipir::Function::Create(functionType, module, name);
            GlobalFunctions[name] = FunctionSymbol(func, method.priv); //TODO: make this better or sum

            if (method.body.empty())
            {
                continue;
            }

            vipir::BasicBlock* entryBasicBlock = vipir::BasicBlock::Create("", func);
            builder.setInsertPoint(entryBasicBlock);

            int index = 0;

            vipir::AllocaInst* alloca = builder.CreateAlloca(vipir::Type::GetPointerType(mType->getVipirType()));
            scope->locals["this"] = LocalSymbol(alloca);

            builder.CreateStore(alloca, func->getArgument(index++));

            for (auto& argument : method.arguments)
            {
                vipir::AllocaInst* alloca = builder.CreateAlloca(argument.type->getVipirType());
                scope->locals[argument.name] = LocalSymbol(alloca);

                builder.CreateStore(alloca, func->getArgument(index++));
            }

            for (auto& node : method.body) {
                node->emit(builder, module, scope);
            }
        }

        return nullptr;
    }

    std::string mangleMethodName(std::string_view structName, std::string_view methodName)
    {
        std::string res = "m";
        res += structName;
        res += methodName;

        return res;
    }
}