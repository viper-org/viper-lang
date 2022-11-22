#include <type/types.hh>

std::map<std::string, std::shared_ptr<Type>> types;

void InitBuiltinTypes(llvm::LLVMContext& ctx)
{
    types["int8"]  = std::make_shared<IntegerType>(8, ctx);
    types["int16"] = std::make_shared<IntegerType>(16, ctx);
    types["int32"] = std::make_shared<IntegerType>(32, ctx);
    types["int64"] = std::make_shared<IntegerType>(64, ctx);

    types["void"]  = std::make_shared<VoidType>(ctx);

    types["bool"]  = std::make_shared<IntegerType>(1, ctx);
}

llvm::Value* Type::Convert(llvm::Value* src, std::shared_ptr<Type> dst, llvm::IRBuilder<>& builder)
{
    if(src->getType() == dst->GetLLVMType())
        return src;
    
    if(src->getType()->isIntegerTy() && dst->IsIntegerTy())
        return builder.CreateSExtOrTrunc(src, dst->GetLLVMType());

    src->mutateType(dst->GetLLVMType()); // If all else fails
    return src;
}