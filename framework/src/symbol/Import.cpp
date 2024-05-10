// Copyright 2024 solar-mist


#include "symbol/Import.h"

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/Parser.h"

#include <fstream>

namespace symbol
{
    ImportManager::ImportManager()
        : mSearchPaths{"./"}
    {
    }

    void ImportManager::addSearchPath(std::string path)
    {
        mSearchPaths.push_back(path);
    }

    std::pair<std::vector<parser::ASTNodePtr>, std::vector<parser::GlobalSymbol>> ImportManager::ImportSymbols(std::filesystem::path path, diagnostic::Diagnostics& diag)
    {
        path += ".vpr";

        std::ifstream stream;

        for (auto searchPath : mSearchPaths)
        {
            stream.open(searchPath / path);
            if (stream.is_open()) break;
        }

        diagnostic::Diagnostics importerDiag;

        std::stringstream buf;
        buf << stream.rdbuf();

        importerDiag.setErrorSender("viper");
        importerDiag.setFileName(path);
        importerDiag.setText(buf.str());
        importerDiag.setImported(true);

        lexing::Lexer lexer(buf.str(), importerDiag);
        auto tokens = lexer.lex();

        parser::Parser parser(tokens, importerDiag, *this, true);
        
        auto nodes = parser.parse();
        return {std::move(nodes), parser.getSymbols()};
    }
}