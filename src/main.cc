#include <compiler.hh>
#include <diagnostics.hh>
#include <optional>

int main(int argc, char** argv)
{
    if(argc < 2)
        Diagnostics::FatalError("viper", "no input files");

    std::string file;
    OutputType outputType = OutputType::Object;
    std::optional<std::string> output;
    for(int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if(arg[0] == '-')
        {
            switch(arg[1])
            {
                case '-':
                    break;
                    // TODO: Parse longer arguments
                case 'S':
                    outputType = OutputType::Assembly;
                    break;
                case 'c':
                    outputType = OutputType::Object;
                    break;
                case 'i':
                    outputType = OutputType::LLVM;
                    break;
                case 'o':
                {
                    if(arg.length() == 2)
                        output = argv[++i];
                    else
                        output = arg.substr(2);
                    break;
                }
                default:
                    Diagnostics::Error("viper", "unrecognized command-line option '\x1b[1m" + arg + "\x1b[0m'");
            }
        }
        else
            file = arg;
    }

    Compiler compile = Compiler(outputType, file, output);
    compile.Compile();

    return 0;
}