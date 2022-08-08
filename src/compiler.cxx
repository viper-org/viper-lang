#include <compiler.hxx>
#include <diagnostics.hxx>
#include <iostream>

namespace Sketch
{
    Compiler::Compiler(SketchOutputType outputType, const std::string inputFileName)
        :_outputType(outputType), _inputFileName(inputFileName), handle(inputFileName)
    {
        if(!handle.is_open())
            Diagnostics::FatalError("qrk", inputFileName + ": No such file or directory");
    }

    SketchOutputType Compiler::getOutputType() const
    {
        return _outputType;
    }

    std::string_view Compiler::getInputFileName() const
    {
        return _inputFileName;
    }
}