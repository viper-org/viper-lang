#ifndef VIPER_CODEGEN_ASSEMBLY_HH
#define VIPER_CODEGEN_ASSEMBLY_HH
#include <codegen/value/immediate.hh>
#include <codegen/value/register.hh>
#include <codegen/value/memory.hh>
#include <codegen/value/cmp.hh>
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
        void CreateExtern(std::string_view ident);
        void CreateLabel(std::string_view label);


        void CreateIndirect(Value* dest, Value* src);


        void CreateJmp(std::string_view label);
    
        void CreatePush(Value* operand);

        void CreateSet(CompareOperator op, Value* operand);

        void CreateMov(Value* dest, Value* src);

        void CreateAdd(Value* dest, Value* src);
        void CreateSub(Value* dest, Value* src);
        void CreateMul(Value* dest, Value* src);
        void CreateDiv(Value* dest, Value* src);

        void CreateCmp(Value* left, Value* right);

        void CreateCall(std::string_view label);

        void CreateLeave();
        void CreateRet();
    private:
        void CreateBinOp(Value* left, Value* right, std::string_view op);

        std::ostringstream _output;

        void VerifyArgs(Value* left, Value* right);
    };
}

#endif