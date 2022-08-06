#include <ast/bin_op.hh>
#include <ast/var.hh>
#include <globals.hh>
#include <iostream>
#include <llvm/IR/Instructions.h>

bin_op_expr::bin_op_expr(token operand, std::unique_ptr<ast_expr> lhs, std::unique_ptr<ast_expr> rhs)
    :lhs(std::move(lhs)), rhs(std::move(rhs))
{
    switch(operand.type)
    {
        case token_type::plus:
            this->operand = binary_operand::PLUS;
            break;
        case token_type::minus:
            this->operand = binary_operand::MINUS;
            break;
        case token_type::star:
            this->operand = binary_operand::MULTIPLY;
            break;
        case token_type::slash:
            this->operand = binary_operand::DIVIDE;
            break;
        case token_type::equal:
            this->operand = binary_operand::EQUAL;
            break;
        case token_type::not_equal:
            this->operand = binary_operand::NOT_EQUAL;
            break;
        case token_type::less_than:
            this->operand = binary_operand::LESS;
            break;
        case token_type::greater_than:
            this->operand = binary_operand::GREATER;
            break;
        case token_type::greater_equal:
            this->operand = binary_operand::GREATER_EQUAL;
            break;
        case token_type::assignment:
            this->operand = binary_operand::ASSIGNMENT;
            break;
        case token_type::increment_assign:
            this->operand = binary_operand::INCREMENT_ASSIGN;
            break;
        case token_type::decrement_assign:
            this->operand = binary_operand::DECREMENT_ASSIGN;
            break;
        default:
            break;
    }
}

std::string bin_op_expr::operand_to_string() const
{
    switch(operand)
    {
        case binary_operand::PLUS:
            return "PLUS";
        case binary_operand::MINUS:
            return "MINUS";
        case binary_operand::MULTIPLY:
            return "MULTIPLY";
        case binary_operand::DIVIDE:
            return "DIVIDE";
        case binary_operand::EQUAL:
            return "EQUAL";
        case binary_operand::NOT_EQUAL:
            return "NOT_EQUAL";
        case binary_operand::LESS:
            return "LESS";
        case binary_operand::GREATER:
            return "GREATER";
        case binary_operand::LESS_EQUAL:
            return "LESS_EQUAL";
        case binary_operand::GREATER_EQUAL:
            return "GREATER_EQUAL";
        case binary_operand::ASSIGNMENT:
            return "ASSIGNMENT";
        case binary_operand::INCREMENT_ASSIGN:
            return "INCREMENT_ASSIGN";
        case binary_operand::DECREMENT_ASSIGN:
            return "DECREMENT_ASSIGN";
    }
    return "";
}

void bin_op_expr::print(std::ostream& stream) const
{
    stream << "<binary-operator>:\nlhs: ";
    lhs->print(stream);
    stream << "\noperand: " << operand_to_string() << "\nrhs: ";
    rhs->print(stream);
}

expr_type bin_op_expr::get_type() const
{
    return expr_type::BIN_OP;
}

llvm::Value* bin_op_expr::codegen(std::shared_ptr<scope> env) const
{
    if(operand == binary_operand::ASSIGNMENT)
    {
        var_expr* left = static_cast<var_expr*>(lhs.get());
        llvm::Value* value = rhs->codegen(env);

        llvm::AllocaInst* alloca = find_named_value(left->get_name(), env);


        if(value->getType() != alloca->getAllocatedType())
            value = type_info::convert(value, alloca->getAllocatedType());
        
        return builder.CreateStore(value, alloca);
    }
    else if(operand == binary_operand::INCREMENT_ASSIGN || operand == binary_operand::DECREMENT_ASSIGN)
    {
        var_expr* left = static_cast<var_expr*>(lhs.get());
        llvm::Value* value = rhs->codegen(env);

        llvm::AllocaInst* alloca = find_named_value(left->get_name(), env);

        if(value->getType() != alloca->getAllocatedType())
            value = type_info::convert(value, alloca->getAllocatedType());
        
        llvm::LoadInst* load = builder.CreateLoad(alloca->getAllocatedType(), alloca, left->get_name().data());

        if(operand == binary_operand::INCREMENT_ASSIGN)
            value = builder.CreateAdd(load, value, "inctmp");
        else
            value = builder.CreateSub(load, value, "dectmp");

        return builder.CreateStore(value, alloca);
    }
    
    llvm::Value* left  = lhs->codegen(env);
    llvm::Value* right = rhs->codegen(env);

    if (left->getType() != right->getType())
    {
        if(left->getType()->getPrimitiveSizeInBits() > right->getType()->getPrimitiveSizeInBits())
            right = builder.CreateSExt(right, left->getType());
        else
            left = builder.CreateSExt(left, right->getType());
    }

    switch(operand)
    {
        case binary_operand::PLUS:
            return builder.CreateAdd(left, right, "addtmp");
        case binary_operand::MINUS:
            return builder.CreateSub(left, right, "subtmp");
        case binary_operand::MULTIPLY:
            return builder.CreateMul(left, right, "multmp");
        case binary_operand::DIVIDE:
            return builder.CreateUDiv(left, right, "divtmp");
        case binary_operand::EQUAL:
            return builder.CreateICmpEQ( left, right, "eqtmp");
        case binary_operand::NOT_EQUAL:
            return builder.CreateICmpNE(left, right, "netmp");
        case binary_operand::LESS:
            return builder.CreateICmpSLT(left, right, "lttmp");
        case binary_operand::GREATER:
            return builder.CreateICmpSGT(left, right, "gttmp");
        default:
            return nullptr;
    }
}