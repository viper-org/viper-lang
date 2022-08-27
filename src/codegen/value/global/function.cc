#include <codegen/value/global/function.hh>

namespace Codegen
{
    Function::Function(const std::string& name, Module& module)
        :Global(module), _name(name)
    {
    }

    const std::string Function::Generate()
    {
        std::string result = "\n\t.globl  " + _name;
        result += "\n\t.type   " + _name + ", @function\n";
        result += _name;

        result += ":\n\tpushq %rbp";
        result += "\n\tmovq %rsp, %rbp";

        for(BasicBlock* basicBlock : _basicBlockList)
        {
            result += basicBlock->Generate();
            delete basicBlock;
        }

        result += "\n\tpopq %rbp";
        result += "\n\tret";
        result += "\n\t.size   " + _name + ", .-" + _name;

        delete this;

        return result;
    }

    Function* Function::Create(const std::string& name, Module& module)
    {
        Function* function = new Function(name, module);

        module.GetGlobals().push_back(static_cast<Global*>(function));

        return function;
    }

    std::vector<BasicBlock*>& Function::GetBasicBlockList()
    {
        return _basicBlockList;
    }
}