// Copyright 2023 solar-mist


#include "type/Type.h"
#include "type/IntegerType.h"
#include "type/PointerType.h"
#include "type/VoidType.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::unique_ptr<Type>> types;

void Type::Init()
{
    types["char"]  = std::make_unique<IntegerType>(8);
    types["short"] = std::make_unique<IntegerType>(16);
    types["int"]   = std::make_unique<IntegerType>(32);
    types["long"]  = std::make_unique<IntegerType>(64);

    types["void"]  = std::make_unique<VoidType>();
}

bool Type::Exists(const std::string& name)
{
    return types.find(name) != types.end();
}

Type* Type::Get(const std::string& name)
{
    return types.at(name).get();
}

Type* Type::GetPointerType(Type* base)
{
    using namespace std::literals;

    std::string key = base->getVipirType()->getName().data() + "*"s;

    if (types.find(key) != types.end())
    {
        return types.at(key).get();
    }

    types.emplace(key, std::make_unique<PointerType>(base));
    return types.at(key).get();
}