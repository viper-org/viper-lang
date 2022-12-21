#include <compiler.hh>
#include <diagnostics.hh>
#include <iostream>
#include <optional>
#include <vector>
#include <sstream>

int main(int argc, char** argv)
{
    if(argc < 2)
        Diagnostics::FatalError("viper", "no input files");

    std::vector<std::string> files;
    std::vector<std::string> objects;

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
                case 'L':
                    outputType = OutputType::Library;
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
        {
            if(arg.substr(arg.length() - 2) == ".o")
                objects.push_back(arg);
            else
                files.push_back(arg);
        }
    }

    if(outputType == OutputType::Library)
    {
        std::vector<std::string> outputs;
        std::stringstream symbols;
        srand(time(0));
        for(std::string& file : files)
        {
            std::string outputFile = "/tmp/";
            for(int i = 0; i < 9; ++i)
                outputFile += ('a' + rand() % 26);
            outputFile += ".o";
            Compiler compiler = Compiler(OutputType::Object, file, outputFile);
            outputs.push_back(outputFile);
            symbols << compiler.Compile();
        }
        outputs.insert(outputs.end(), objects.begin(), objects.end());
        Compiler::CompileLibrary(outputs, symbols, output.value_or("out.vplib"));
        for(std::string& file : outputs)
            std::remove(file.data());
    }
    else
    {
        Compiler compiler = Compiler(outputType, files[0], output);
        compiler.Compile();
    }

    return 0;
}