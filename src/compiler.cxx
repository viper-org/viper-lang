#include <compiler.hxx>
#include <diagnostics.hxx>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>
#include <sstream>

namespace Viper
{
    Compiler::Compiler(ViperOutputType outputType, const std::string inputFileName)
        :_builder(_context), _module(inputFileName, _context), _outputType(outputType), _inputFileName(inputFileName), _handle(inputFileName)
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

    void Compiler::Compile()
    {
        InitDefaultTypes();
        _lexer = std::make_unique<Lexing::Lexer>(_contents);
        
        _parser = std::make_unique<Parsing::Parser>(_lexer->Lex(), _contents);

        for(std::unique_ptr<Parsing::ASTTopLevel>& node : _parser->Parse())
            node->Generate(_context, _builder, _module);
        
        if(_outputType == ViperOutputType::LLVM)
        {
            std::string outputFileName = _inputFileName + ".ll";
            std::error_code ec;
            llvm::raw_fd_ostream dest = llvm::raw_fd_ostream(outputFileName, ec, llvm::sys::fs::OF_None);
            if(ec)
            {
                llvm::errs() << "Could not open file: " << ec.message();
                std::exit(1);
            }

            _module.print(dest, nullptr);
        }
        else if(_outputType == ViperOutputType::Assembly || _outputType == ViperOutputType::Object)
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

            std::string outputFileName;
            if(_outputType == ViperOutputType::Assembly)
                outputFileName = _inputFileName + ".s";
            else
                outputFileName = _inputFileName + ".o";
            
            std::error_code ec;
            llvm::raw_fd_ostream dest = llvm::raw_fd_ostream(outputFileName, ec, llvm::sys::fs::OF_None);

            if(ec)
            {
                llvm::errs() << "Could not open file: " << ec.message();
                std::exit(1);
            }

            llvm::legacy::PassManager pass;
            [[maybe_unused]] llvm::CodeGenFileType fileType;
            if(_outputType == ViperOutputType::Assembly)
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
    }

    ViperOutputType Compiler::getOutputType() const
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