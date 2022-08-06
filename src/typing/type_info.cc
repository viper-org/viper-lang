#include <typing/type_info.hh>
#include <globals.hh>
#include <llvm/IR/DerivedTypes.h>
#include <string>
#include <unordered_map>

std::unordered_map<std::string_view, type_info> types;

type_info::type_info()
{
}

type_info::type_info(std::function<llvm::Type*(llvm::LLVMContext&)> llvm_getter, std::string_view name, std::string_view llvm_name, unsigned int size)
    :llvm_getter(llvm_getter), name(name), llvm_name(llvm_name), size(size)
{
}

void type_info::init_default_types()
{
    types["i8"] =   type_info(llvm::Type::getInt8Ty,  "i8",   "i8",   1);
    types["i16"] =  type_info(llvm::Type::getInt16Ty, "i16",  "i16",  2);
    types["i32"] =  type_info(llvm::Type::getInt32Ty, "i32",  "i32",  4);
    types["i64"] =  type_info(llvm::Type::getInt64Ty, "i64",  "i64",  8);
    types["bool"] = type_info(llvm::Type::getInt1Ty,  "bool", "i1",   1);
    types["char"] = type_info(llvm::Type::getInt8Ty,  "char", "i8",   0);
    types["void"] = type_info(llvm::Type::getVoidTy,  "void", "void", 0);
}