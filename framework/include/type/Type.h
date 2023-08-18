// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_TYPE_H
#define VIPER_FRAMEWORK_TYPE_TYPE_H 1

#include "vipir/Type/Type.h"

class Type
{
public:
    virtual ~Type() { }

    virtual int getSize() const = 0;
    virtual vipir::Type* getVipirType() const = 0;

    static void Init();
    static bool Exists(const std::string& name);
    static Type* Get(const std::string& name);
};

#endif