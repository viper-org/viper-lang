#include "type/type.hh"
#include <type/types.hh>

std::map<std::string, std::shared_ptr<Type>> types;

void InitBuiltinTypes(llvm::LLVMContext& ctx)
{
    types.clear();
    types["int8"]  = std::make_shared<IntegerType>(8, ctx);
    types["int16"] = std::make_shared<IntegerType>(16, ctx);
    types["int32"] = std::make_shared<IntegerType>(32, ctx);
    types["int64"] = std::make_shared<IntegerType>(64, ctx);

    types["void"]  = std::make_shared<VoidType>(ctx);

    types["bool"]  = std::make_shared<IntegerType>(1, ctx);
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
        return Type(_llvmType->getPointerElementType()).GetMangleID() + "P";
    if(_llvmType->isVoidTy())
        return "V";
    if(_llvmType->isArrayTy())
        return std::to_string(_llvmType->getArrayNumElements()) + Type(_llvmType->getArrayElementType()).GetMangleID();
    if(_llvmType->isStructTy())
    {
        std::string res = "S" + std::to_string(_llvmType->getStructName().str().length()) + _llvmType->getStructName().str() + std::to_string(_llvmType->getStructNumElements());
        for(unsigned int i = 0; i < _llvmType->getStructNumElements(); i++)
            res += Type(_llvmType->getStructElementType(i)).GetMangleID();
        
        return res;
    }
    return "ERR";
}