#include <symbol/Symbol.h>

Symbol::Symbol(bool isLocal, Type* type, const std::string& name)
    : mIsLocal(isLocal)
    , mType(type)
    , mName(name)
{
}

bool Symbol::getIsLocal() const
{
    return mIsLocal;
}

Type* Symbol::getType() const
{
    return mType;
}

std::string_view Symbol::getName() const
{
    return mName;
}