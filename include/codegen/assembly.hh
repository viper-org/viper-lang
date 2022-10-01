#ifndef VIPER_CODEGEN_ASSEMBLY_HH
#define VIPER_CODEGEN_ASSEMBLY_HH
#include <codegen/value/immediate.hh>
#include <codegen/value/register.hh>
#include <sstream>
#include <memory>

namespace Codegen
{
    class Assembly
    {
    public:
        Assembly();

        void Emit(std::ostream& stream);

        void CreateGlobal(std::string_view ident);
        void CreateLabel(std::string_view ident);

        void CreateMov(std::unique_ptr<Value> left, std::unique_ptr<Value> right);

        void CreateRet();
    private:
        std::ostringstream _output;

        void VerifyArgs(std::unique_ptr<Value>& left, std::unique_ptr<Value>& right);
    };
}

#endif