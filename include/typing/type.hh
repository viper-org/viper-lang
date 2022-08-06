#ifndef QRK_TYPE_HH
#define QRK_TYPE_HH
#include <string>
#include <memory>
#include <unordered_map>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/ADT/Twine.h>
#include <globals.hh>

enum class type_type
{
    base,
    pointer,
    array,
};

class quark_type
{
protected:
    llvm::Type* llvm_type;
    std::string_view name;
    std::string_view llvm_name;
    unsigned int size;
public:
    quark_type(llvm::Type*, std::string_view, std::string_view, unsigned int);

    type_type T;

    virtual llvm::Type* get_type() const;
    virtual unsigned int get_size() const;
    virtual std::string_view get_name() const;

    static llvm::Value* convert(llvm::Value*, llvm::Type*, llvm::Twine = "");
    static std::shared_ptr<quark_type> find(std::string_view);
    static void init_default_types();

    static std::shared_ptr<quark_type> i8_t;
    static std::shared_ptr<quark_type> i16_t;
    static std::shared_ptr<quark_type> i32_t;
    static std::shared_ptr<quark_type> i64_t;
    static std::shared_ptr<quark_type> bool_t;
    static std::shared_ptr<quark_type> char_t;
    static std::shared_ptr<quark_type> void_t;
};

extern std::unordered_map<std::string_view, std::shared_ptr<quark_type>> types;

#endif