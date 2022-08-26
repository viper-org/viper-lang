#ifndef VIPER_CODEGEN_FUNCTION_HH
#define VIPER_CODEGEN_FUNCTION_HH
#include <codegen/value/global/global.hh>

namespace Codegen
{
    class Function : public Global
    {
    public:
        static Function* Create(const std::string& name, Module& module);
    protected:
        Function(const std::string& name, Module& module);
        const std::string Generate() override;
    private:
        std::string _name;
        std::vector<Value*>& _body;
    };
}

#endif