#ifndef VIPER_COMPILER_HH
#define VIPER_COMPILER_HH
#include <codegen/builder.hh>
#include <codegen/module.hh>
#include <string>
#include <fstream>

enum class OutputType
{
    Assembly,
    // TODO: Add more output types
};

class Compiler
{
public:
    Compiler(OutputType outputType, const std::string& inputFileName);

    void Compile();

private:
    OutputType _outputType;

    Codegen::Module _module;
    Codegen::Builder _builder;

    std::ifstream _inputHandle;
    std::string _contents;
};

#endif