#include <compiler.hh>
#include <diagnostics.hh>

int main(int argc, char** argv)
{
    if(argc < 2)
        Diagnostics::FatalError("viper", "no input files");

    Compiler compile = Compiler(OutputType::LLVM, argv[1]);
    compile.Compile();

    return 0;
}