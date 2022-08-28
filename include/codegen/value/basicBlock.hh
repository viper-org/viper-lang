#ifndef VIPER_CODEGEN_BASICBLOCK_HH
#define VIPER_CODEGEN_BASICBLOCK_HH
#include <codegen/value/value.hh>
#include <codegen/value/instruction/instruction.hh>

namespace Codegen
{
    class Function;
    class BasicBlock : public Value
    {
    public:
        static BasicBlock* Create(Module& module, const std::string& name = "", Function* parent = nullptr);

        std::vector<Instruction*>& GetInstrList();
        Function*   GetParent() const;
        std::string GetName()   const;
        Module&     GetModule() const;

        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;

    private:
        BasicBlock(Module& module, const std::string& name = "", Function* parent = nullptr);

        Module& _module;
        std::string _name;
        Function* _parent;
        std::vector<Instruction*> _instructions;
    };
}

#endif