#ifndef VIPER_CODEGEN_FUNCTION_HH
#define VIPER_CODEGEN_FUNCTION_HH
#include <codegen/value/global/global.hh>
#include <codegen/value/basicBlock.hh>

namespace Codegen
{
    class Function : public Global
    {
    public:
        static Function* Create(const std::string& name, Module& module);

        std::vector<BasicBlock*>& GetBasicBlockList();
    protected:
        Function(const std::string& name, Module& module);
        const std::string Generate() override;
    private:
        std::string _name;
        std::vector<BasicBlock*> _basicBlockList;
    };
}

#endif