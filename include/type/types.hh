#ifndef VIPER_TYPES_HH
#define VIPER_TYPES_HH
#include <type/type.hh>
#include <type/integerType.hh>
#include <type/voidType.hh>
#include <type/pointerType.hh>
#include <type/arrayType.hh>
#include <type/structType.hh>
#include <llvm/IR/LLVMContext.h>
#include <map>
#include <memory>

extern std::map<std::string, std::shared_ptr<Type>> types;

void InitBuiltinTypes(llvm::LLVMContext& ctx);

#endif