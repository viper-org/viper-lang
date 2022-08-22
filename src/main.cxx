#include <compiler.hxx>
#include <diagnostics.hxx>
#include <types/types.hxx>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
        Viper::Diagnostics::FatalError("viper", "no input files");
    
    Viper::OutputType outputType = Viper::OutputType::LLVM;
    std::vector<std::string> files;
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
                    outputType = Viper::OutputType::Assembly;
                    break;
                case 'c':
                    outputType = Viper::OutputType::Object;
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
                    Viper::Diagnostics::Error("viper", "unrecognized command-line option '\x1b[1m" + arg + "\x1b[0m'");
            }
        }
        else
            files.push_back(arg);
    }

    if(output.has_value())
    {
        if(files.size() > 1)
            Viper::Diagnostics::FatalError("viper", "cannot specify '\x1b[1m-o\x1b[0m' with multiple files");
        
        Viper::Compiler compiler(outputType, files[0], output);
        compiler.Compile();
    }
    else
    {
        for(std::string& file : files)
        {
            Viper::Compiler compiler(outputType, file, std::nullopt);
            compiler.Compile();
        }
    }
}