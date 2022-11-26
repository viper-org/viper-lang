#include <compiler.hh>
#include <lexing/lexer.hh>
#include <llvm/MC/MCExpr.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/MC/MCContext.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>
#include <parsing/parser.hh>
#include <diagnostics.hh>
#include <sstream>
#include <iostream>

Compiler::Compiler(OutputType outputType, const std::string& inputFileName, const std::optional<std::string>& outputFileName)
    :_outputType(outputType), _inputFileName(inputFileName)
{
    _inputHandle = std::ifstream(inputFileName);
    if(!_inputHandle.is_open())
        Diagnostics::FatalError("viper", inputFileName + ": No such file or directory");
    Diagnostics::setFileName(inputFileName);
    
    std::stringstream buf;
    buf << _inputHandle.rdbuf();
    _contents = buf.str() + '\n';

    _inputHandle.close();

    if(outputFileName.has_value())
        _outputFileName = outputFileName.value();
    else
    {
        std::string suffix;
        if(outputType == OutputType::LLVM)
            suffix = ".ll";
        else if(outputType == OutputType::Assembly)
            suffix = ".s";
        else
            suffix = ".o";
        _outputFileName = _inputFileName + suffix;
    }
}

void Compiler::Compile()
{
    llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder = llvm::IRBuilder(ctx);
    llvm::Module mod(_inputFileName, ctx);

    mod.setPIELevel(llvm::PIELevel::Level::Large);
    mod.setPICLevel(llvm::PICLevel::Level::BigPIC);

    InitBuiltinTypes(ctx);

    Lexing::Lexer* lexer = new Lexing::Lexer(_contents);
    Parsing::Parser* parser = new Parsing::Parser(lexer->Lex(), _contents, ctx);

    std::vector<std::unique_ptr<Parsing::ASTNode>> ast = parser->Parse();

    for(std::unique_ptr<Parsing::ASTNode>& node : ast)
        node->Emit(ctx, mod, builder, nullptr);
    delete parser;
    delete lexer;

    if(_outputType == OutputType::LLVM)
    {
        std::error_code ec;
        llvm::raw_fd_ostream dest(_outputFileName, ec, llvm::sys::fs::OF_None);

        if (ec) {
            llvm::errs() << "Could not open file: " << ec.message();
            std::exit(1);
        }
        mod.print(dest, nullptr);
        return;
    }

    std::string targetTriple = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

    if (!target) {
        llvm::errs() << error;
        std::exit(1);
    }

    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    llvm::TargetMachine* targetMachine = target->createTargetMachine(targetTriple, CPU, features, opt, llvm::Reloc::PIC_);

    mod.setDataLayout(targetMachine->createDataLayout());
    mod.setTargetTriple(targetTriple);

    std::error_code ec;
    llvm::raw_fd_ostream dest(_outputFileName, ec, llvm::sys::fs::OF_None);

    if (ec) {
        llvm::errs() << "Could not open file: " << ec.message();
        std::exit(1);
    }

    llvm::legacy::PassManager pass;
    llvm::CodeGenFileType FileType = (_outputType == OutputType::Assembly) ? llvm::CGFT_AssemblyFile : llvm::CGFT_ObjectFile;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        std::exit(1);
    }

    pass.run(mod);
    dest.flush();

    delete targetMachine;
}