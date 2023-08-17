// Copyright 2023 solar-mist


#include "parser/ast/global/Function.h"

namespace parsing
{
    Function::Function(const std::string& name, std::vector<ASTNodePtr> body)
        : mName(name)
        , mBody(std::move(body))
    {
    }

    std::string_view Function::getName() const
    {
        return mName;
    }

    const std::vector<ASTNodePtr>& Function::getBody() const
    {
        return mBody;
    }
}