#include <compiler.hxx>
#include <diagnostics.hxx>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
        Sketch::Diagnostics::FatalError("qra", "no input files");
    
    Sketch::Compiler compiler(Sketch::SketchOutputType::LLVM, argv[1]);
}