#ifndef VIPER_CODEGEN_CMP_HH
#define VIPER_CODEGEN_CMP_HH
#include <codegen/value/value.hh>
#include <string>

namespace Codegen
{
    enum class CompareOperator
    {
        EQ, NE,
        LT, GT,
        LE, GE,
    };

    class Compare : public Value
    {
    public:
        Compare(CompareOperator op);

        std::string Emit(int bits) override;
        int GetSize() const override;
        void Dispose() override;
        bool IsCompare() override;

        CompareOperator GetOperator() const;
    private:
        CompareOperator _operator;
    };
}

#endif