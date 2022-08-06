#include <exception>
#include <typing/type_info.hh>
#include <globals.hh>
#include <llvm/IR/DerivedTypes.h>
#include <string>
#include <unordered_map>

std::unordered_map<std::string_view, type_info> types;

type_info::type_info()
{
}

type_info::type_info(std::function<llvm::Type*(llvm::LLVMContext&)> llvm_getter, std::function<llvm::Type*(llvm::LLVMContext&, unsigned int)> llvm_ptr_getter,
                     std::string_view name, std::string_view llvm_name, unsigned int size)
    :name(name), size(size), ptr(false)
{
    llvm_info.getter = llvm_getter;
    llvm_info.ptr_getter = llvm_ptr_getter;
    llvm_info.name = llvm_name;
}

llvm::Type* type_info::get_llvm_type() const
{
    if(ptr)
        return llvm_info.ptr_getter(ctx, 0);
    else
        return llvm_info.getter(ctx);
}

unsigned int type_info::get_size()
{
    if(ptr)
        return 8;
    else
        return size;
}

llvm::Value* type_info::convert(llvm::Value* value, llvm::Type* type, llvm::Twine twine)
{
    if(type->isIntegerTy() && value->getType()->isIntegerTy())
        return builder.CreateSExtOrTrunc(value, type, twine);
    
    value->mutateType(type);
    return value;
}

void type_info::init_default_types()
{
    types["i8"] =   type_info(llvm::Type::getInt8Ty,  llvm::Type::getInt8PtrTy,  "i8",   "i8",   1);
    types["i16"] =  type_info(llvm::Type::getInt16Ty, llvm::Type::getInt16PtrTy, "i16",  "i16",  2);
    types["i32"] =  type_info(llvm::Type::getInt32Ty, llvm::Type::getInt32PtrTy, "i32",  "i32",  4);
    types["i64"] =  type_info(llvm::Type::getInt64Ty, llvm::Type::getInt64PtrTy, "i64",  "i64",  8);
    types["bool"] = type_info(llvm::Type::getInt1Ty,  llvm::Type::getInt1PtrTy,  "bool", "i1",   1);
    types["char"] = type_info(llvm::Type::getInt8Ty,  llvm::Type::getInt8PtrTy,  "char", "i8",   1);
    types["void"] = type_info(llvm::Type::getVoidTy,  llvm::Type::getInt8PtrTy,  "void", "void", 0);
}