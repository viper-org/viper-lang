// Copyright 2024 solar-mist


#include "symbol/Identifier.h"

#include <algorithm>
#include <unordered_map>

namespace symbol
{
    struct Identifier
    {
        std::string mangledName;

        std::vector<std::string> names;
    };

    std::vector<Identifier> identifiers;

    void AddIdentifier(std::string mangledName, std::vector<std::string> names)
    {
        if (std::find_if(identifiers.begin(), identifiers.end(), [&mangledName](const auto& ident){
                return ident.mangledName == mangledName;
            }) == identifiers.end())
        {
            identifiers.push_back({std::move(mangledName), std::move(names)});
        }
    }

    std::vector<std::string> GetSymbol(std::vector<std::string> givenNames, std::vector<std::string> activeNames)
    {
        std::vector<std::string> ret;

        while(true)
        {
            for (auto& ident : identifiers)
            {
                if (ident.names == givenNames)
                {
                    ret.push_back(ident.mangledName);
                }
            }
            if (!activeNames.empty())
            {
                givenNames.insert(givenNames.begin(), activeNames.back());
                activeNames.pop_back();
            }
            else break;
        }

        return ret;
    }
}