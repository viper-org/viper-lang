#ifndef QRK_TYPE_INFO_HXX
#define QRK_TYPE_INFO_HXX
#include <string>
#include <functional>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/ADT/Twine.h>

struct type_info
{
    type_info();
    type_info(std::function<llvm::Type*(llvm::LLVMContext&)>, std::function<llvm::Type*(llvm::LLVMContext&, unsigned int)>, std::string_view, std::string_view, unsigned int);
    struct {
        std::function<llvm::Type*(llvm::LLVMContext&)> getter;
        std::function<llvm::Type*(llvm::LLVMContext&, unsigned int)> ptr_getter;
        std::string_view name;
    } llvm_info;
    std::string_view name;
    unsigned int size;
    bool ptr;

    static llvm::Value* convert(llvm::Value*, llvm::Type*, llvm::Twine = "");
    static void init_default_types();
};

#endif