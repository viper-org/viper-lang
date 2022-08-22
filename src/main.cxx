#include <compiler.hxx>
#include <diagnostics.hxx>
#include <types/types.hxx>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
        Viper::Diagnostics::FatalError("viper", "no input files");
    
    Viper::Compiler compiler(Viper::ViperOutputType::LLVM, argv[1]);

    compiler.Compile();
}