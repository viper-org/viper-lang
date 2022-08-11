#include <compiler.hxx>
#include <diagnostics.hxx>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
        Viper::Diagnostics::FatalError("qra", "no input files");
    
    Viper::Compiler compiler(Viper::ViperOutputType::LLVM, argv[1]);

    for(llvm::Value* node : compiler.Compile())
    {
        llvm::outs() << *node;
    }
}