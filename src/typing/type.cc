#include <typing/type.hh>

std::unordered_map<std::string_view, std::shared_ptr<quark_type>> types;

std::shared_ptr<quark_type> quark_type::i8_t =   std::make_shared<quark_type>(llvm::Type::getInt8Ty(ctx),  "i8",   "i8",   1);
std::shared_ptr<quark_type> quark_type::i16_t =  std::make_shared<quark_type>(llvm::Type::getInt16Ty(ctx), "i16",  "i16",  2);
std::shared_ptr<quark_type> quark_type::i32_t =  std::make_shared<quark_type>(llvm::Type::getInt32Ty(ctx), "i32",  "i32",  4);
std::shared_ptr<quark_type> quark_type::i64_t =  std::make_shared<quark_type>(llvm::Type::getInt64Ty(ctx), "i64",  "i64",  8);
std::shared_ptr<quark_type> quark_type::bool_t = std::make_shared<quark_type>(llvm::Type::getInt1Ty(ctx),  "bool", "i1",   1);
std::shared_ptr<quark_type> quark_type::char_t = std::make_shared<quark_type>(llvm::Type::getInt8Ty(ctx),  "char", "i8",   1);
std::shared_ptr<quark_type> quark_type::void_t = std::make_shared<quark_type>(llvm::Type::getVoidTy(ctx),  "void", "void", 0);

quark_type::quark_type(llvm::Type* llvm_type, std::string_view name, std::string_view llvm_name, unsigned int size)
    :llvm_type(llvm_type), name(name), llvm_name(llvm_name), size(size)
{
}

llvm::Type* quark_type::get_type() const
{
    return llvm_type;
}

unsigned int quark_type::get_size() const
{
    return size;
}

std::string_view quark_type::get_name() const
{
    return llvm_name;
}

llvm::Value* quark_type::convert(llvm::Value* value, llvm::Type* type, llvm::Twine twine)
{
    if(type->isIntegerTy() && value->getType()->isIntegerTy())
        return builder.CreateSExtOrTrunc(value, type, twine);
    
    value->mutateType(type);
    return value;
}

std::shared_ptr<quark_type> quark_type::find(std::string_view name)
{
    if(types.find(name) != types.end())
        return types.at(name);
    
    return nullptr;
}

void quark_type::init_default_types()
{
    types["i8"]   = i8_t =   std::make_shared<quark_type>(llvm::Type::getInt8Ty(ctx),  "i8",   "i8",   1);
    types["i16"]  = i16_t =  std::make_shared<quark_type>(llvm::Type::getInt16Ty(ctx), "i16",  "i16",  2);
    types["i32"]  = i32_t =  std::make_shared<quark_type>(llvm::Type::getInt32Ty(ctx), "i32",  "i32",  4);
    types["i64"]  = i64_t =  std::make_shared<quark_type>(llvm::Type::getInt64Ty(ctx), "i64",  "i64",  8);
    types["bool"] = bool_t = std::make_shared<quark_type>(llvm::Type::getInt1Ty(ctx),  "bool", "i1",   1);
    types["char"] = char_t = std::make_shared<quark_type>(llvm::Type::getInt8Ty(ctx),  "char", "i8",   1);
    types["void"] = void_t = std::make_shared<quark_type>(llvm::Type::getVoidTy(ctx),  "void", "void", 0);
}