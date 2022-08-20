#include <types/types.hxx>

namespace Viper
{
    std::map<std::string_view, std::shared_ptr<Type>> types;

    void InitDefaultTypes()
    {
        types["i8"]  = std::make_shared<IntegerType>(8);
        types["i16"] = std::make_shared<IntegerType>(16);
        types["i32"] = std::make_shared<IntegerType>(32);
        types["i64"] = std::make_shared<IntegerType>(64);
    }

    llvm::Value* Type::Convert(llvm::Value* value, llvm::Type* type, llvm::IRBuilder<>& builder)
    {
        if(value->getType() == type)
            return value;
        
        if(type->getPrimitiveSizeInBits() == 1)
            return builder.CreateIsNotNull(value);

        if(value->getType()->isIntegerTy() && type->isIntegerTy())
            return builder.CreateIntCast(value, type, false);

        value->mutateType(type);
        return value;
    }
}