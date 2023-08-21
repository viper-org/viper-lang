// Copyright 2023 solar-mist


#include "parser/ast/expression/UnaryExpression.h"

#include <vipir/IR/Instruction/AddrOfInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

#include <vipir/Module.h>

namespace parsing
{
    UnaryExpression::UnaryExpression(ASTNodePtr operand, lexing::TokenType op)    
        : mOperand(std::move(operand))
    {
        switch (op)
        {
            case lexing::TokenType::Asperand:
                mOperator = Operator::AddressOf;
                break;
            case lexing::TokenType::Star:
                mOperator = Operator::Dereference;
                break;
            
            default:
                break;
        }
    }

    vipir::Value* UnaryExpression::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        vipir::Value* operand = mOperand->emit(builder, module, scope);

        switch (mOperator)
        {
            case Operator::AddressOf:
            {
                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(operand);
                instruction->eraseFromParent();
                return builder.CreateAddrOf(static_cast<vipir::AllocaInst*>(vipir::getPointerOperand(operand)));
            }
            case Operator::Dereference:
            {
                return builder.CreateLoad(operand);
            }
        }
    }
}