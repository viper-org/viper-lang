#include <compiler.hh>
#include <diagnostics.hh>

int main(int argc, char** argv)
{
    if(argc < 2)
        diagnostics::fatal_error("viper", "no input files");

    compiler compile = compiler(outputType::assembly, argv[1]);
    compile.compile();

    return 0;
}