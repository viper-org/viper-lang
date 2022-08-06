#include <iostream>
#include <fstream>
#include <llvm/ADT/Optional.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/LegacyPassManager.h>
#include <sstream>
#include <parser.hh>
#include <llvm/IR/Function.h>

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        /*std::string target_triple = llvm::sys::getDefaultTargetTriple();
        llvm::InitializeAllTargetInfos();
        llvm::InitializeAllTargets();
        llvm::InitializeAllTargetMCs();
        llvm::InitializeAllAsmParsers();
        llvm::InitializeAllAsmPrinters();

        std::string error;
        const llvm::Target* target = llvm::TargetRegistry::lookupTarget(target_triple, error);

        if(!target)
        {
            llvm::errs() << error;
            return 1;
        }

        std::string cpu = "generic";
        std::string features = "";

        llvm::TargetOptions opt;
        llvm::Optional<llvm::Reloc::Model> rm = llvm::Optional<llvm::Reloc::Model>();
        llvm::TargetMachine* target_machine = target->createTargetMachine(target_triple, cpu, features, opt, rm);

        module.setDataLayout(target_machine->createDataLayout());
        module.setTargetTriple(target_triple);*/

        std::ifstream file(argv[1]);
        std::ofstream out_file(std::string(argv[1]) + ".s");
        std::stringstream buf;
        buf << file.rdbuf();
        quark_type::init_default_types();
        auto ast = parser::parse(buf.str());

        for(extern_func& node : ast.externs)
        {
            std::string buffer;
            llvm::raw_string_ostream stream(buffer);
            node.codegen()->print(stream);
            out_file << buffer;
        }
        
        for(function_ast& node : ast.functions)
        {
            std::string buffer;
            llvm::raw_string_ostream stream(buffer);
            node.codegen()->print(stream);
            out_file << buffer;

        }
        /*std::error_code error_code;
            llvm::raw_fd_ostream dest(llvm::StringRef(out_file_name), error_code);

            llvm::legacy::PassManager pass;
            llvm::CodeGenFileType file_type = llvm::CGFT_AssemblyFile;

            if(target_machine->addPassesToEmitFile(pass, dest, nullptr, file_type))
            {
                llvm::errs() << "Target machine cannot emit a file of this type.";
                return 1;
            }

            pass.run(module);
            dest.flush();*/
        return 0;
    }
    return 1;
}