// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_SYMBOL_NAME_MANGLING_H
#define VIPER_FRAMEWORK_SYMBOL_NAME_MANGLING_H

#include "parser/ast/Node.h"

#include "diagnostic/Diagnostic.h"

#include <filesystem>
#include <vector>

namespace parser
{
    class Symbol;
}

namespace symbol
{
    class ImportManager
    {
    public:
        ImportManager();

        void addSearchPath(std::string path);
        std::pair<std::vector<parser::ASTNodePtr>, std::vector<parser::Symbol>> ImportSymbols(std::filesystem::path path, diagnostic::Diagnostics& diag);

    private:
        std::vector<std::string> mSearchPaths;
    };

}

#endif //VIPER_FRAMEWORK_SYMBOL_NAME_MANGLING_H