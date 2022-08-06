#ifndef QRK_TYPE_INFO_HXX
#define QRK_TYPE_INFO_HXX
#include <string>
#include <functional>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>

struct type_info
{
    type_info();
    type_info(std::function<llvm::Type*(llvm::LLVMContext&)>, std::string_view, std::string_view, unsigned int);
    std::function<llvm::Type*(llvm::LLVMContext&)> llvm_getter;
    std::string_view name;
    std::string_view llvm_name;
    unsigned int size;

    static void init_default_types();
};

#endif