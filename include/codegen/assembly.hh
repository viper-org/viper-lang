#ifndef VIPER_CODEGEN_ASSEMBLY_HH
#define VIPER_CODEGEN_ASSEMBLY_HH
#include <codegen/value/immediate.hh>
#include <codegen/value/register.hh>
#include <codegen/value/memory.hh>
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
        void CreateLabel(std::string_view label);

        void CreateJmp(std::string_view label);
    
        void CreatePush(Value* operand);

        void CreateMov(Value* left, Value*right);

        void CreateAdd(Value* left, Value* right);
        void CreateSub(Value* left, Value* right);
        void CreateMul(Value* left, Value* right);
        void CreateDiv(Value* left, Value* right);

        void CreateLeave();
        void CreateRet();
    private:
        void CreateBinOp(Value* left, Value* right, std::string_view op);

        std::ostringstream _output;

        void VerifyArgs(Value* left, Value* right);
    };
}

#endif