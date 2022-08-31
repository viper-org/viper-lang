#include <type/integerType.hh>

IntegerType::IntegerType(int bits)
    :Type("int" + std::to_string(bits), bits)
{
    switch(bits)
    {
        case 64:
            _instructionSuffix = "q";
            break;
        case 32:
            _instructionSuffix = "l";
            break;
        case 16:
            _instructionSuffix = "w";
            break;
        case 8:
            _instructionSuffix = "b";
            break;
    }
}

bool IntegerType::IsIntegerTy() const
{
    return true;
}