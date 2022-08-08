#include <compiler.hxx>
#include <diagnostics.hxx>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
        Quark::Diagnostics::FatalError("qra", "no input files");
    
    Quark::Compiler compiler(Quark::QuarkOutputType::LLVM, argv[1]);
}