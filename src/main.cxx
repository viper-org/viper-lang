#include <compiler.hxx>
#include <diagnostics.hxx>
#include <types/types.hxx>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2)
        Viper::Diagnostics::FatalError("viper", "no input files");
    
    Viper::OutputType outputType = Viper::OutputType::Executable;

    std::vector<std::string> files;
    std::vector<std::string> objects;
    std::vector<std::string> libraries;

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
                case 'L':
                    outputType = Viper::OutputType::Library;
                    break;
                case 'i':
                    outputType = Viper::OutputType::LLVM;
                    break;
                case 'l':
                {
                    std::string lib;
                    if(arg.length() == 2)
                        lib = argv[++i];
                    else
                        lib = arg.substr(2);
                    if(lib.length() < 7)
                        libraries.push_back(lib + ".vplib");
                    else if(lib.substr(7) != ".vplib")
                        libraries.push_back(lib + ".vplib");
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
                    Viper::Diagnostics::Error("viper", "unrecognized command-line option '\x1b[1m" + arg + "\x1b[0m'");
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

    if(outputType == Viper::OutputType::Library)
    {
        std::vector<std::string> outputFiles;
        std::vector<std::unique_ptr<Viper::CodeGen::Symbol>> symbols;
        srand(time(0));
        for(std::string& file : files)
        {
            std::string outputFile = "/tmp/";
            for(int i = 0; i < 9; ++i)
                outputFile += ('a' + rand() % 26);
            outputFile += ".o";
            Viper::Compiler compiler(Viper::OutputType::Object, file, outputFile, libraries);
            std::vector<std::unique_ptr<Viper::CodeGen::Symbol>> fileSymbols = compiler.Compile().first;
            for(auto& symbol : fileSymbols)
                symbols.push_back(std::move(symbol));
            outputFiles.push_back(outputFile);
        }
        outputFiles.insert(outputFiles.end(), objects.begin(), objects.end());
        Viper::Compiler::GenerateLibrary(outputFiles, std::move(symbols), output.value_or("out.vplib"));
        for(std::string& file : outputFiles)
            std::remove(file.data());
    }
    else if(outputType == Viper::OutputType::Executable)
    {
        std::vector<std::string> objectFiles;
        for(std::string& file : files)
        {
            Viper::Compiler compiler(outputType, file, output, libraries);
            objectFiles.push_back(compiler.Compile().second);
        }
        objectFiles.insert(objectFiles.end(), objects.begin(), objects.end());
        Viper::Compiler::GenerateExecutable(objectFiles, libraries, output.value_or("a.out"));
    }
    else
    {
        for(std::string& file : files)
        {
            Viper::Compiler compiler(outputType, file, output, libraries);
            compiler.Compile();
        }
    }
}