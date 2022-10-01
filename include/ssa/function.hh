#ifndef VIPER_SSA_FUNCTION_HH
#define VIPER_SSA_FUNCTION_HH
#include <ssa/value.hh>
#include <memory>

namespace SSA
{
    class Function : public Value
    {
    public:
        Function(const std::string& name, std::unique_ptr<Value>& body);

        void Print(std::ostream& stream, int indent) const override;
    private:
        std::string _name;
        std::unique_ptr<Value> _body;
    };
}

#endif