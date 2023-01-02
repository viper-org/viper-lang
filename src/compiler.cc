#include "parsing/ast/astNode.hh"
#include "type/structType.hh"
#include "type/types.hh"
#include <compiler.hh>
#include <lexing/lexer.hh>
#include <llvm/IR/LLVMContext.h>
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
#include <fstream>

Compiler::Compiler(OutputType outputType, const std::string& inputFileName, const std::optional<std::string>& outputFileName, const std::vector<std::string>& libraries)
    :_outputType(outputType), _inputFileName(inputFileName), _outputFileName(outputFileName), _libraries(libraries)
{
    _inputHandle = std::ifstream(inputFileName);
    if(!_inputHandle.is_open())
        Diagnostics::FatalError("viper", inputFileName + ": No such file or directory");
    Diagnostics::setFileName(inputFileName);
    
    std::stringstream buf;
    buf << _inputHandle.rdbuf();
    _contents = buf.str() + '\n';

    _inputHandle.close();
}

std::string Compiler::GetSymbols()
{
    StructType::ResetStructTypeArray();
    llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder = llvm::IRBuilder(ctx);
    llvm::Module mod(_inputFileName, ctx);
    InitBuiltinTypes(ctx);
    
    std::vector<std::shared_ptr<VarSymbol>> importedSymbols;

    for(const std::string& lib : _libraries)
    {
        std::ifstream file(lib.data());
        std::stringstream buf;
        buf << file.rdbuf();
        std::string text = buf.str().substr(0, buf.str().find_first_of(0x0A));
        Lexing::Lexer lexer(text);
        Parsing::Parser parser(lexer.Lex(), text, ctx, {}, true);
        std::vector<std::pair<std::unique_ptr<Parsing::ASTNode>, std::shared_ptr<VarSymbol>>> declarations = parser.ParseLibrary();
        for(std::pair<std::unique_ptr<Parsing::ASTNode>, std::shared_ptr<VarSymbol>>& decl : declarations)
        {
            decl.first->Emit(ctx, mod, builder, {});
            importedSymbols.push_back(decl.second);
        }
        
    }

    Lexing::Lexer lexer(_contents);
    Parsing::Parser parser(lexer.Lex(), _contents, ctx, importedSymbols, true);

    std::string outputFileName;

    std::vector<std::unique_ptr<Parsing::ASTNode>> ast = parser.Parse();

    std::string symbols;
    for(std::unique_ptr<Parsing::ASTNode>& node : ast)
    {
        if(Parsing::Function* def = dynamic_cast<Parsing::Function*>(node.get()))
        {
            symbols += "@";
            symbols += def->GetMangledName();
        }
    }
    symbols += StructType::EmitStructSymbols();
    StructType::ResetStructTypeArray();
    types.clear();

    return symbols;
}

std::pair<std::string, std::string> Compiler::Compile(std::string symbols)
{
    StructType::ResetStructTypeArray();
    llvm::LLVMContext ctx;
    llvm::IRBuilder<> builder = llvm::IRBuilder(ctx);
    llvm::Module mod(_inputFileName, ctx);

    mod.setPIELevel(llvm::PIELevel::Level::Large);
    mod.setPICLevel(llvm::PICLevel::Level::BigPIC);

    InitBuiltinTypes(ctx);

    std::vector<std::shared_ptr<VarSymbol>> importedSymbols;

    for(const std::string& lib : _libraries)
    {
        std::ifstream file(lib.data());
        std::stringstream buf;
        buf << file.rdbuf();
        std::string text = buf.str().substr(0, buf.str().find_first_of(0x0A));
        Lexing::Lexer lexer(text);
        Parsing::Parser parser(lexer.Lex(), text, ctx, {}, true);
        std::vector<std::pair<std::unique_ptr<Parsing::ASTNode>, std::shared_ptr<VarSymbol>>> declarations = parser.ParseLibrary();
        for(std::pair<std::unique_ptr<Parsing::ASTNode>, std::shared_ptr<VarSymbol>>& decl : declarations)
        {
            decl.first->Emit(ctx, mod, builder, {});
            importedSymbols.push_back(decl.second);
        }
        
    }
    Lexing::Lexer symbolLexer(symbols);
    Parsing::Parser symbolParser(symbolLexer.Lex(), symbols, ctx, {}, true);
    std::vector<std::pair<std::unique_ptr<Parsing::ASTNode>, std::shared_ptr<VarSymbol>>> declarations = symbolParser.ParseLibrary();
    for(std::pair<std::unique_ptr<Parsing::ASTNode>, std::shared_ptr<VarSymbol>>& decl : declarations)
    {
        decl.first->Emit(ctx, mod, builder, {});
        importedSymbols.push_back(decl.second);
    }

    Lexing::Lexer* lexer = new Lexing::Lexer(_contents);
    Parsing::Parser* parser = new Parsing::Parser(lexer->Lex(), _contents, ctx, importedSymbols);

    std::string outputFileName;

    std::vector<std::unique_ptr<Parsing::ASTNode>> ast = parser->Parse();

    for(std::unique_ptr<Parsing::ASTNode>& node : ast)
        node->Emit(ctx, mod, builder, nullptr);

    if(_outputType == OutputType::LLVM)
    {
        outputFileName = _outputFileName.value_or(_inputFileName + ".ll");
        std::error_code ec;
        llvm::raw_fd_ostream dest(outputFileName, ec, llvm::sys::fs::OF_None);
        if (ec) {
            llvm::errs() << "Could not open file: " << ec.message();
            std::exit(1);
        }
        mod.print(dest, nullptr);
        return {"", outputFileName};
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

    if(_outputType == OutputType::Assembly)
        outputFileName = _outputFileName.value_or(_inputFileName + ".s");
    else if(_outputType == OutputType::Object)
        outputFileName = _outputFileName.value_or(_inputFileName + ".o");
    else
        outputFileName = "/tmp/" + _inputFileName + ".o";

    mod.setDataLayout(targetMachine->createDataLayout());
    mod.setTargetTriple(targetTriple);

    std::error_code ec;
    llvm::raw_fd_ostream dest(outputFileName, ec, llvm::sys::fs::OF_None);

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


    std::string retSymbols = StructType::EmitStructSymbols();

    for(llvm::Function& func : mod.functions())
    {
        std::string name = func.getName().str();
        if(name.find('.') != name.npos)
            name = name.substr(0, name.find_first_of('.'));

        retSymbols += "@" + name;
    }

    delete targetMachine;
    delete parser;
    delete lexer;
    types.clear();

    return {retSymbols, outputFileName};
}

void Compiler::CompileLibrary(const std::vector<std::string>& objects, const std::stringstream& symbols, std::string_view output)
{
    std::ofstream out(output.data());
    out << symbols.str();
    out << (char)0x0A;
    for(const std::string& object : objects)
    {
        unsigned int fileNameIndex = object.find_last_of('/') + 1;
        std::string newFileName = "/tmp/" + object.substr(fileNameIndex);
        out << (newFileName + std::string(16 - newFileName.size(), 0));
        std::ifstream input(object);
        std::stringstream buf;
        buf << input.rdbuf();
        std::string length = std::to_string(buf.str().length());
        out << length << std::string(8 - length.length(), 0) << (char)0x0A;
        out << buf.str() << (char)0x0A;
    }
    out.flush();
}

void Compiler::CompileExecutable(std::vector<std::string>& objectFiles, const std::vector<std::string>& libraries, const std::string& outputFileName)
{
    std::string command = "ld -o " + outputFileName;
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
    for(const std::string& object : objectFiles)
        command += " " + object;
    
    system(command.data());
    for(const std::string& object : objectFiles)
        std::remove(object.data());
}