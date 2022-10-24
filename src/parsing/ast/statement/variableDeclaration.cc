#include <parsing/ast/statement/variableDeclaration.hh>

namespace Parsing
{
    VariableDeclaration::VariableDeclaration(const std::string& name, std::shared_ptr<Type> type, std::unique_ptr<ASTNode> initVal, Environment* scope, std::optional<std::vector<std::pair<std::shared_ptr<Type>, std::string>>> args)
        :ASTNode(ASTNodeType::VariableDeclaration), _name(name), _initVal(std::move(initVal)), _scope(scope), _args(std::move(args))
    {
        _nodeType = (_args.has_value() ? ASTNodeType::Function : ASTNodeType::VariableDeclaration);
        _type = type;
    }

    void VariableDeclaration::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << (_args.has_value() ? "<Function>:\n" : "<Variable-Declaration>:\n");
        stream << std::string(indent, ' ') << "Name: " << _name;
        if(_initVal)
        {
            stream << "\n" << std::string(indent, ' ') << "Value: \n";
            _initVal->Print(stream, indent + 2);
        }
    }

    bool VariableDeclaration::IsFunction() const
    {
        return _scope != nullptr;
    }

    SSA::Value* VariableDeclaration::Emit(SSA::Builder& builder, Environment* scope, bool)
    {
        if(_scope)
        {
            std::vector<SSA::AllocaInst*> args;
            for(std::pair<std::shared_ptr<Type>, std::string> arg : _args.value())
            {
                SSA::AllocaInst* alloca = builder.CreateAlloca(arg.first, "", true);
                args.push_back(alloca);
                _scope->GetNamedValues()[arg.second] = alloca;
            }
            SSA::Function* func = SSA::Function::Create(builder.GetModule(), _name, args);
            SSA::BasicBlock* entryBB = SSA::BasicBlock::Create(builder.GetModule(), func);
            builder.SetInsertPoint(entryBB);
            _initVal->Emit(builder, _scope, true);

            delete _scope;

            return func;
        }
        SSA::AllocaInst* alloca = builder.CreateAlloca(_type);
        if(_initVal)
        {
            SSA::Value* initVal = _initVal->Emit(builder, scope);
            builder.CreateStore(alloca, initVal);
        }

        scope->GetNamedValues()[_name] = alloca;
        
        return alloca;
    }
}