// Copyright 2023 solar-mist


#include "type/Type.h"
#include "type/IntegerType.h"
#include "type/VoidType.h"

#include <unordered_map>

std::unordered_map<std::string, std::unique_ptr<Type>> types;

void Type::Init()
{
    types["i8"]  = std::make_unique<IntegerType>(8);
    types["i16"] = std::make_unique<IntegerType>(16);
    types["i32"] = std::make_unique<IntegerType>(32);
    types["i64"] = std::make_unique<IntegerType>(64);

    types["void"] = std::make_unique<VoidType>();
}

bool Type::Exists(const std::string& name)
{
    return types.find(name) != types.end();
}

Type* Type::Get(const std::string& name)
{
    return types.at(name).get();
}