#ifndef SKETCH_COMPILER_HXX
#define SKETCH_COMPILER_HXX
#include <string>
#include <fstream>

namespace Sketch
{
    enum class SketchOutputType
    {
        LLVM,
        Assembly,
        Object,
    };

    class Compiler
    {
    public:
        Compiler(SketchOutputType outputType, const std::string inputFileName = "a.out");

        SketchOutputType getOutputType() const;
        std::string_view getInputFileName() const;
    private:
        SketchOutputType _outputType;
        std::string _inputFileName;

        std::ifstream handle;
    };
}

#endif