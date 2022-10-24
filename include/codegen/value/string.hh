#ifndef VIPER_CODEGEN_STRING_HH
#define VIPER_CODEGEN_STRING_HH
#include <codegen/value/value.hh>
#include <type/types.hh>

namespace Codegen
{
    class StringLiteral : public Value
    {
    public:
        StringLiteral(const std::string& value, const std::string& name);

        std::string Emit(int bits) override;

        bool IsMemory() override;

        int GetSize() const override;

        void Dispose() override;

        std::string_view GetValue() const;
        std::string_view GetName() const;
    private:
        std::string _value;
        std::string _name;
    };
}

#endif