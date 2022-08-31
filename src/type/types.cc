#include <type/types.hh>
#include <codegen/builder.hh>
#include <codegen/value/value.hh>

std::map<std::string, Type*> types = {
    {"int8",  new IntegerType(8)},
    {"int16", new IntegerType(16)},
    {"int32", new IntegerType(32)},
    {"int64", new IntegerType(64)},
};

Codegen::Value* Type::Convert(Codegen::Value* value, Type* type, Codegen::Builder& builder)
{
    if(value->GetType()->IsIntegerTy() && type->IsIntegerTy())
    {
        return builder.CreateSExt(value, type);
    }
    throw;
}