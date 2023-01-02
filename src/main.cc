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
    std::vector<std::string> libraries;

    OutputType outputType = OutputType::Executable;
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
                case 'l':
                {
                    std::string lib;
                    if(arg.length() == 2)
                        lib = argv[++i];
                    else
                        lib = arg.substr(2);
                    if(lib.length() < 7)
                        libraries.push_back(lib + ".vlib");
                    else if(lib.substr(7) != ".vlib")
                        libraries.push_back(lib + ".vlib");
                    else
                        libraries.push_back(lib);
                    break;
                }
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
    if(output.has_value() && files.size() > 1)
    {
        if(outputType != OutputType::Executable && outputType != OutputType::Library)
            Diagnostics::FatalError("viper", "cannot specify '\x1b[1m-o\x1b[0m' with '\x1b[1m-c\x1b[0m', '\x1b[1m-S\x1b[0m' or '\x1b[1m-i\x1b[0m' with multiple files");
    }

    std::stringstream symbols;
    for(std::string& file : files)
    {
        Compiler compiler(OutputType::LLVM, file, file, libraries);
        symbols << compiler.GetSymbols();
    }

    if(outputType == OutputType::Library)
    {
        std::vector<std::string> outputs;
        std::stringstream libSymbols;
        srand(time(0));
        for(std::string& file : files)
        {
            std::string outputFile = "/tmp/";
            for(int i = 0; i < 9; ++i)
                outputFile += ('a' + rand() % 26);
            outputFile += ".o";
            Compiler compiler = Compiler(OutputType::Object, file, outputFile, libraries);
            outputs.push_back(outputFile);
            libSymbols << compiler.Compile(symbols.str()).first;
        }
        outputs.insert(outputs.end(), objects.begin(), objects.end());
        Compiler::CompileLibrary(outputs, libSymbols, output.value_or("out.vlib"));
        for(std::string& file : outputs)
            std::remove(file.data());
    }
    else if(outputType == OutputType::Executable)
    {
        std::vector<std::string> objectFiles;
        for(std::string& file : files)
        {
            Compiler compiler(outputType, file, output, libraries);
            objectFiles.push_back(compiler.Compile(symbols.str()).second);
        }
        objectFiles.insert(objectFiles.end(), objects.begin(), objects.end());
        Compiler::CompileExecutable(objectFiles, libraries, output.value_or("a.out"));
    }
    else
    {
        Compiler compiler = Compiler(outputType, files[0], output, libraries);
        compiler.Compile(symbols.str());
    }

    return 0;
}