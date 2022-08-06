#include <globals.hh>

llvm::LLVMContext                   ctx;
llvm::IRBuilder<>                   builder(ctx);
llvm::Module                        module("quark", ctx); 