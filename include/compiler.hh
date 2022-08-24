#ifndef VIPER_COMPILER_HH
#define VIPER_COMPILER_HH
#include <string>
#include <fstream>

enum class outputType
{
    assembly,
    // TODO: Add more output types
};

class compiler
{
public:
    compiler(outputType output_type, const std::string& input_file_name);

    void compile();

private:
    outputType output_type;
    std::string input_file_name;

    std::ifstream input_handle;
    std::string contents;
};

#endif