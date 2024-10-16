#ifndef VIPER_TESTS_COMPILER_VIPER_ASSERT_H
#define VIPER_TESTS_COMPILER_VIPER_ASSERT_H 1

#include <format>
#include <string>
#include <source_location>

enum class AssertOperator {
    None = 0,
    EQ,
    NEQ,
    LE,
    LT,
    GE,
    GT,
};

enum class AssertResult {
    None,
    True,
    False,
};

struct AssertInfo {
    const char* expr;
    std::source_location sourceLocation;
    AssertOperator op;
    AssertResult result;
    std::string lhs;
    std::string rhs;
};

template<typename T>
concept formattable = requires (T& v, std::format_context ctx) { std::formatter<std::remove_cvref_t<T>>().format(v, ctx); };

template <class A>
struct AssertCheck {
    A a;
    AssertInfo& i;

    template <class B>
    AssertInfo operator==(B&& b) &&
    {
        i.result = (a == b) ? AssertResult::True : AssertResult::False;

        if constexpr (formattable<A>)
        {
            i.lhs = std::format("{}", a);
        }

        if constexpr (formattable<B>)
        {
            i.rhs = std::format("{}", b);
        }

        i.op = AssertOperator::EQ;
        return std::move(i);
    }
    
    template <class B>
    AssertInfo operator!=(B&& b) &&
    {
        i.result = (a != b) ? AssertResult::True : AssertResult::False;

        if constexpr (formattable<A>)
        {
            i.lhs = std::format("{}", a);
        }

        if constexpr (formattable<B>)
        {
            i.rhs = std::format("{}", b);
        }

        i.op = AssertOperator::NEQ;
        return std::move(i);
    }
};

struct AssertDecomposer {
    AssertInfo i;
    template <class A>
    AssertCheck<A> operator<(A&& a) && {
        return AssertCheck<A>{std::forward<A>(a), i};
    }
};

#endif // VIPER_TESTS_VIPER_ASSERT_H