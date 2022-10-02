#include "environment.hh"
#include "ssa/value/basicBlock.hh"
#include <parsing/ast/statement/variableDeclaration.hh>

namespace Parsing
{
    VariableDeclaration::VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, bool isFunction)
        :_name(name), _initVal(std::move(initVal)), _isFunction(isFunction)
    {
        _nodeType = (_isFunction ? ASTNodeType::Function : ASTNodeType::VariableDeclaration);
    }

    void VariableDeclaration::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << (_isFunction ? "<Function>:\n" : "<Variable-Declaration>:\n");
        stream << std::string(indent, ' ') << "Name: " << _name;
        if(_initVal)
        {
            stream << "\n" << std::string(indent, ' ') << "Value: \n";
            _initVal->Print(stream, indent + 2);
        }
    }

    bool VariableDeclaration::IsFunction() const
    {
        return _isFunction;
    }

    SSA::Value* VariableDeclaration::Emit(SSA::Builder& builder)
    {
        if(_isFunction)
        {
            SSA::Function* func = SSA::Function::Create(builder.GetModule(), _name);
            SSA::BasicBlock* entryBB = SSA::BasicBlock::Create(builder.GetModule(), func);
            builder.SetInsertPoint(entryBB);
            _initVal->Emit(builder);

            return func;
        }
        SSA::AllocaInst* alloca = builder.CreateAlloca();
        if(_initVal)
        {
            SSA::Value* initVal = _initVal->Emit(builder);
            builder.CreateStore(alloca, initVal);
        }

        namedValues[_name] = alloca;
        
        return alloca;
    }
}