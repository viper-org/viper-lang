#include <iostream>
#include <parsing/ast/expression/binaryExpression.hh>
#include <parsing/ast/expression/variable.hh>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <diagnostics.hh>

namespace Parsing
{
    BinaryExpression::BinaryExpression(std::unique_ptr<ASTNode> lhs, Lexing::Token op, std::unique_ptr<ASTNode> rhs)
        :ASTNode(ASTNodeType::BinaryExpression), _lhs(std::move(lhs)), _rhs(std::move(rhs))
    {
        switch(op.GetType())
        {
            case Lexing::TokenType::Plus:
                _operator = BinaryOperator::Addition;
                break;
            case Lexing::TokenType::Minus:
                _operator = BinaryOperator::Subtraction;
                break;
            case Lexing::TokenType::Star:
                _operator = BinaryOperator::Multiplication;
                break;
            case Lexing::TokenType::Slash:
                _operator = BinaryOperator::Division;
                break;

            case Lexing::TokenType::DoubleEquals:
                _operator = BinaryOperator::Equal;
                break;
            case Lexing::TokenType::BangEquals:
                _operator = BinaryOperator::NotEqual;
                break;

            case Lexing::TokenType::LeftAngleBracket:
                _operator = BinaryOperator::LessThan;
                break;
            case Lexing::TokenType::RightAngleBracket:
                _operator = BinaryOperator::GreaterThan;
                break;

            case Lexing::TokenType::Equals:
                _operator = BinaryOperator::Assignment;
                break;
            case Lexing::TokenType::PlusEquals:
                _operator = BinaryOperator::AddAssign;
                break;
            case Lexing::TokenType::MinusEquals:
                _operator = BinaryOperator::SubAssign;
                break;
            case Lexing::TokenType::StarEquals:
                _operator = BinaryOperator::MulAssign;
                break;
            case Lexing::TokenType::SlashEquals:
                _operator = BinaryOperator::DivAssign;
                break;
                
            case Lexing::TokenType::LeftSquareBracket:
                _operator = BinaryOperator::Subscript;
                break;
            case Lexing::TokenType::Dot:
                _operator = BinaryOperator::MemberAccess;
                break;
                
            case Lexing::TokenType::As:
                _operator = BinaryOperator::TypeConvert;
                break;
            default:
                break;
        }
    }

    std::string BinaryExpression::OperatorToString() const
    {
        switch(_operator)
        {
            case BinaryOperator::Addition:
                return "Addition";
            case BinaryOperator::Subtraction:
                return "Subtraction";
            case BinaryOperator::Multiplication:
                return "Multiplication";
            case BinaryOperator::Division:
                return "Division";
            case BinaryOperator::Equal:
                return "Equal";
            case BinaryOperator::NotEqual:
                return "NotEqual";
            case BinaryOperator::LessThan:
                return "LessThan";
            case BinaryOperator::GreaterThan:
                return "GreaterThan";
            case BinaryOperator::Assignment:
                return "Assignment";
            case BinaryOperator::Subscript:
                return "Subscript";
            case BinaryOperator::MemberAccess:
                return "MemberAccess";
            case BinaryOperator::LogicalAnd:
                return "LogicalAnd";
            case BinaryOperator::LogicalOr:
                return "LogicalOr";
            case BinaryOperator::AddAssign:
                return "AddAssign";
            case BinaryOperator::SubAssign:
                return "SubAssign";
            case BinaryOperator::MulAssign:
                return "MulAssign";
            case BinaryOperator::DivAssign:
                return "DivAssign";
            case BinaryOperator::TypeConvert:
                return "TypeConvert";
        }
        return "";
    }

    BinaryOperator BinaryExpression::GetOperator() const
    {
        return _operator;
    }

    void BinaryExpression::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Binary-Expression>:\n";
        stream << std::string(indent, ' ') << "Lhs: ";
        _lhs->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "\nOperator: " << OperatorToString() << "\n";
        stream << std::string(indent, ' ') << "Rhs: ";
        _rhs->Print(stream, indent + 2);
    }

    void BinaryExpression::AssignType()
    {
        if(_operator == BinaryOperator::Assignment)
            _type = _lhs->GetType();
        else if(_operator == BinaryOperator::Subscript)
            _type = _lhs->GetType()->GetBase();
        else if(_operator == BinaryOperator::MemberAccess)
        {
            if(!_lhs->GetType() || (!_lhs->GetType()->IsStructTy() && !_lhs->GetType()->GetBase()->IsStructTy()));
            else if(_lhs->GetType()->IsPointerTy())
                _type = std::make_shared<Type>(static_cast<StructType*>(_lhs->GetType()->GetBase().get())->GetMemberIndex(static_cast<Variable*>(_rhs.get())->GetName()).second);
            else
                _type = std::make_shared<Type>(static_cast<StructType*>(_lhs->GetType().get())->GetMemberIndex(static_cast<Variable*>(_rhs.get())->GetName()).second);
        }
        else if(_operator == BinaryOperator::Equal || _operator == BinaryOperator::NotEqual || _operator == BinaryOperator::GreaterThan || _operator == BinaryOperator::LessThan)
            _type = types.at("bool");
        else
            _type = _lhs->GetType();
    }

    llvm::Value* BinaryExpression::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        llvm::Value* left = _lhs->Emit(ctx, mod, builder, scope);

        if(!_type)
            AssignType();

        if(_operator == BinaryOperator::MemberAccess)
        {
            if(_lhs->GetType()->IsPointerTy())
            {
                std::pair field = static_cast<StructType*>(_lhs->GetType()->GetBase().get())->GetMemberIndex(static_cast<Variable*>(_rhs.get())->GetName());
                
                llvm::Value* gep = builder.CreateStructGEP(left->getType()->getNonOpaquePointerElementType(), left, field.first);

                llvm::Value* load = builder.CreateLoad(gep->getType()->getNonOpaquePointerElementType(), gep);

                return load;
            }
            else
            {
                std::pair field = static_cast<StructType*>(_lhs->GetType().get())->GetMemberIndex(static_cast<Variable*>(_rhs.get())->GetName());
                llvm::Instruction* inst = static_cast<llvm::Instruction*>(left);
                llvm::Value* ptr = llvm::getPointerOperand(left);
                
                llvm::Value* gep = builder.CreateStructGEP(ptr->getType()->getNonOpaquePointerElementType(), ptr, field.first);

                llvm::Value* load = builder.CreateLoad(gep->getType()->getNonOpaquePointerElementType(), gep);

                inst->eraseFromParent();

                return load;
            }
        }

        llvm::Value* right;
        if(_operator != BinaryOperator::TypeConvert)
            right = _rhs->Emit(ctx, mod, builder, scope);
        
        switch(_operator)
        {
            case BinaryOperator::Addition:
                if(_type->IsPointerTy())
                    return builder.CreateInBoundsGEP(_type->GetLLVMType()->getPointerElementType(), left, right);
                return builder.CreateAdd(left, right);
            case BinaryOperator::Subtraction:
                return builder.CreateSub(left, right);
            case BinaryOperator::Multiplication:
                return builder.CreateMul(left, right);
            case BinaryOperator::Division:
                return builder.CreateSDiv(left, right);

            case BinaryOperator::LogicalAnd:
                return builder.CreateLogicalAnd(left, right);
            case BinaryOperator::LogicalOr:
                return builder.CreateLogicalOr(left, right);

            case BinaryOperator::Equal:
                return builder.CreateICmpEQ(left, right);
            case BinaryOperator::NotEqual:
                return builder.CreateICmpNE(left, right);
            
            case BinaryOperator::LessThan:
                return builder.CreateICmpSLT(left, right);
            case BinaryOperator::GreaterThan:
                return builder.CreateICmpSGT(left, right);
            
            case BinaryOperator::Assignment:
            {
                llvm::Instruction* inst = static_cast<llvm::Instruction*>(left);
                llvm::Value* ptr = llvm::getPointerOperand(left);
                inst->eraseFromParent();
                return builder.CreateStore(right, ptr);
            }
            case BinaryOperator::AddAssign:
            {
                llvm::Value* ptr = llvm::getPointerOperand(left);
                llvm::Value* value;
                if(_type->IsPointerTy())
                    value =  builder.CreateInBoundsGEP(_type->GetLLVMType()->getPointerElementType(), left, right);
                else
                    value = builder.CreateNSWAdd(left, right);
                return builder.CreateStore(value, ptr);
            }
            case BinaryOperator::SubAssign:
            {
                llvm::Value* ptr = llvm::getPointerOperand(left);
                llvm::Value* add = builder.CreateNSWSub(left, right);
                return builder.CreateStore(add, ptr);
            }
            case BinaryOperator::MulAssign:
            {
                llvm::Value* ptr = llvm::getPointerOperand(left);
                llvm::Value* add = builder.CreateNSWMul(left, right);
                return builder.CreateStore(add, ptr);
            }
            case BinaryOperator::DivAssign:
            {
                llvm::Value* ptr = llvm::getPointerOperand(left);
                llvm::Value* add = builder.CreateSDiv(left, right);
                return builder.CreateStore(add, ptr);
            }

            case BinaryOperator::Subscript:
            {
                llvm::Instruction* inst = static_cast<llvm::Instruction*>(left);
                llvm::Value* ptr = llvm::getPointerOperand(inst);

                llvm::Value* gep = builder.CreateInBoundsGEP(left->getType(), ptr, {llvm::ConstantInt::get(ctx, llvm::APInt(64, 0)), right});
                llvm::Value* load = builder.CreateLoad(gep->getType()->getPointerElementType(), gep);

                inst->eraseFromParent();

                return load;
            }

            case BinaryOperator::TypeConvert:
            {
                llvm::Type* llvmType = _type->GetLLVMType();

                if(left->getType() == llvmType)
                    return left;

                if(llvmType->isIntegerTy())
                {
                    if(left->getType()->isIntegerTy())
                    {
                        if(llvmType->isIntegerTy(1))
                            return builder.CreateIsNotNull(left);
                        return builder.CreateSExtOrTrunc(left, llvmType);
                    }
                    else if(left->getType()->isPointerTy())
                        return builder.CreatePtrToInt(left, llvmType);
                }
                else if(llvmType->isPointerTy())
                {
                    if(left->getType()->isPointerTy())
                        return builder.CreatePointerCast(left, llvmType);
                    else if(left->getType()->isIntegerTy())
                        return builder.CreateIntToPtr(left, llvmType);
                    else if(left->getType()->isArrayTy())
                    {
                        llvm::Instruction* inst = static_cast<llvm::Instruction*>(left);
                        llvm::Value* ptr = llvm::getPointerOperand(inst);

                        return builder.CreateInBoundsGEP(left->getType(), ptr, {llvm::ConstantInt::get(ctx, llvm::APInt(64, 0)), llvm::ConstantInt::get(ctx, llvm::APInt(64, 0))});
                    }
                }
            }
            default:
                return nullptr;
        }
    }
}