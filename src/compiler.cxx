#include <compiler.hxx>
#include <diagnostics.hxx>
#include <llvm/BinaryFormat/ELF.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>
#include <sstream>
#include <string>

namespace Viper
{
    Compiler::Compiler(OutputType outputType, const std::string& inputFileName, const std::optional<std::string>& outputFileName, const std::vector<std::string>& libraries)
        :_builder(_context), _module(inputFileName, _context), _outputType(outputType), _inputFileName(inputFileName), _outputFileName(outputFileName), _libraries(libraries), _handle(inputFileName)
    {
        if(!_handle.is_open())
            Diagnostics::FatalError("viper", inputFileName + ": No such file or directory");

        Diagnostics::setFileName(_inputFileName);
        _module.setSourceFileName(_inputFileName);

        _targetTriple = llvm::sys::getDefaultTargetTriple();
        _module.setTargetTriple(_targetTriple);

        _module.setPICLevel(llvm::PICLevel::Level::BigPIC);
        _module.setPIELevel(llvm::PIELevel::Level::Large);

        std::stringstream buf;
        buf << _handle.rdbuf();
        _contents = buf.str();
    }

    std::pair<std::vector<std::unique_ptr<CodeGen::Symbol>>, std::string> Compiler::Compile()
    {
        std::vector<std::unique_ptr<CodeGen::Symbol>> symbols;
        InitDefaultTypes();
        _lexer = std::make_unique<Lexing::Lexer>(_contents);
        
        _parser = std::make_unique<Parsing::Parser>(_lexer->Lex(), _contents, _libraries);

        for(std::unique_ptr<Parsing::ASTTopLevel>& node : _parser->Parse())
        {
            symbols.push_back(node->Generate(_context, _builder, _module).second);
        }
        
        std::string outputFileName;
        
        if(_outputType == OutputType::LLVM)
        {
            outputFileName = _outputFileName.value_or(_inputFileName + ".ll");
            
            std::error_code ec;
            llvm::raw_fd_ostream dest = llvm::raw_fd_ostream(outputFileName, ec, llvm::sys::fs::OF_None);
            if(ec)
            {
                llvm::errs() << "Could not open file: " << ec.message();
                std::exit(1);
            }

            _module.print(dest, nullptr);
        }
        else if(_outputType == OutputType::Assembly || _outputType == OutputType::Object || _outputType == OutputType::Executable)
        {
            llvm::InitializeAllTargetInfos();
            llvm::InitializeAllTargets();
            llvm::InitializeAllTargetMCs();
            llvm::InitializeAllAsmParsers();
            llvm::InitializeAllAsmPrinters();

            std::string err;
            const llvm::Target* target = llvm::TargetRegistry::lookupTarget(_targetTriple, err);
            if(!target)
            {
                llvm::errs() << err;
                std::exit(1);
            }

            std::string cpu = "generic";
            std::string features = "";

            llvm::TargetOptions options;
            llvm::Optional<llvm::Reloc::Model> rm = llvm::Optional<llvm::Reloc::Model>();
            llvm::TargetMachine* targetMachine = target->createTargetMachine(_targetTriple, cpu, features, options, rm);

            if(_outputType == OutputType::Assembly)
                outputFileName = _outputFileName.value_or(_inputFileName + ".s");
            else if(_outputType == OutputType::Object)
                outputFileName = _outputFileName.value_or(_inputFileName + ".o");
            else
                outputFileName = "/tmp/" + _inputFileName + ".o";
            
            std::error_code ec;
            llvm::raw_fd_ostream dest = llvm::raw_fd_ostream(outputFileName, ec, llvm::sys::fs::OF_None);

            if(ec)
            {
                llvm::errs() << "Could not open file: " << ec.message();
                std::exit(1);
            }

            llvm::legacy::PassManager pass;
            [[maybe_unused]] llvm::CodeGenFileType fileType;
            if(_outputType == OutputType::Assembly)
                fileType = llvm::CGFT_AssemblyFile;
            else
                fileType = llvm::CGFT_ObjectFile;

            if(targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
            {
                llvm::errs() << "TargetMachine cannot emit a file of this type";
                std::exit(1);
            }

            pass.run(_module);
            dest.flush();
            delete targetMachine;
        }
        return std::make_pair(std::move(symbols), outputFileName);
    }

    void Compiler::GenerateExecutable(std::vector<std::string> objectFiles, const std::vector<std::string>& libraries, const std::string& outputFileName)
    {
        std::string command = "ld -o ./" + outputFileName;
        for(const std::string& library : libraries)
        {
            std::ifstream file(library);
            std::stringstream buf;
            buf << file.rdbuf();

            unsigned int nameStart = 0;
            unsigned int size = 0;
            unsigned int offset = 0;
            while(nameStart + 26 + size < buf.str().length())
            {
                nameStart = buf.str().find_first_of(0x0A, offset) + 1;
                size = std::stoi(buf.str().substr(nameStart + 16, 8));
                
                std::ofstream out(buf.str().substr(nameStart, 16));
                if(out.is_open())
                {
                    objectFiles.push_back(buf.str().substr(nameStart, 16));
                    out << buf.str().substr(nameStart + 25, size);
                }
                offset = nameStart + size - 7;
            }
        }
        for(const std::string& file : objectFiles)
            command += " " + file;

        llvm::outs() << command << "\n";
        system(command.data()); // TODO: Remove call to system and implement a proper linking solution
        for(const std::string& file : objectFiles)
            std::remove(file.data());
    }

    void Compiler::GenerateLibrary(const std::vector<std::string>& objectFiles, const std::vector<std::unique_ptr<Viper::CodeGen::Symbol>>& symbols, const std::string& outputFileName)
    {
        std::ofstream outFile(outputFileName);
        for(const std::unique_ptr<CodeGen::Symbol>& symbol : symbols)
        {
            if(symbol)
                outFile << symbol->ToString();
        }
        outFile << (char)0x0A;
        for(std::string file : objectFiles)
        {
            unsigned int fileNameIndex = file.find_last_of('/') + 1;
            std::string newFileName = "/tmp/" + file.substr(fileNameIndex);
            outFile << (newFileName + std::string(16 - newFileName.size(), 0));
            std::ifstream input(file);
            std::stringstream buf;
            buf << input.rdbuf();
            std::string length = std::to_string(buf.str().length());
            outFile << length << std::string(8 - length.length(), 0) << (char)0x0A;
            outFile << buf.str() << (char)0x0A;
        }
        outFile.flush();
    }

    OutputType Compiler::getOutputType() const
    {
        return _outputType;
    }

    std::string_view Compiler::getInputFileName() const
    {
        return _inputFileName;
    }

    std::string_view Compiler::getFileContents() const
    {
        return _contents;
    }
}