#include "type/type.hh"
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

llvm::Value* Type::Convert(llvm::Value* src, llvm::Type* dst, llvm::IRBuilder<>& builder)
{
    if(src->getType() == dst)
        return src;

    if(src->getType()->isIntegerTy() && dst->isIntegerTy(1))
        return builder.CreateIsNotNull(src);
    
    if(src->getType()->isIntegerTy() && dst->isIntegerTy())
        return builder.CreateSExtOrTrunc(src, dst);

    if(src->getType()->isIntegerTy() && dst->isPointerTy())
        return builder.CreateIntToPtr(src, dst);
    
    if(src->getType()->isPointerTy() && dst->isIntegerTy())
        return builder.CreatePtrToInt(src, dst);

    src->mutateType(dst); // If all else fails
    return src;
}

std::string Type::GetMangleID()
{
    if(_llvmType->isIntegerTy())
    {
        switch(_llvmType->getScalarSizeInBits())
        {
            case 64:
                return "q";
            case 32:
                return "i";
            case 16:
                return "w";
            case 8:
                return "c";
            case 1:
                return "b";
        }
    }
    if(_llvmType->isPointerTy())
        return "P" + Type(_llvmType->getPointerElementType()).GetMangleID();
    if(_llvmType->isVoidTy())
        return "V";
    if(_llvmType->isArrayTy())
        return std::to_string(_llvmType->getArrayNumElements()) + Type(_llvmType->getArrayElementType()).GetMangleID();
    if(_llvmType->isStructTy())
    {
        std::string res = "S" + std::to_string(_llvmType->getStructNumElements());
        for(unsigned int i = 0; i < _llvmType->getStructNumElements(); i++)
            res += Type(_llvmType->getStructElementType(i)).GetMangleID();
        
        return res;
    }
    return "ERR";
}