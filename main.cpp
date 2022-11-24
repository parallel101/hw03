#include <iostream>
#include <vector>
#include <variant>
#include <cpp_type_name.h>

// 请修复这个函数的定义：10 分
template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &a) {
    os << "{";
    for (size_t i = 0; i < a.size(); i++) {
        os << a[i];
        if (i != a.size() - 1)
            os << ", ";
    }
    os << "}";
    return os;
}

// 请修复这个函数的定义：10 分
template <class T1, class T2>
std::vector<decltype(T1{} + T2{})>
operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    // 当列表不等长时，取较短的
    size_t len = std::min(a.size(), b.size());
    std::vector<decltype(T1{} + T2{})> ret;
    for (size_t i = 0; i < len; ++i)
        ret.push_back(a[i] + b[i]);
    return ret;
}

template <class T1, class T2, class U>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, U &&b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    using decay_U = std::decay_t<U>;
    if constexpr (std::is_same_v<T1, decay_U> || std::is_same_v<T2, decay_U>)  {
        return std::visit([&b](auto &&A) -> std::variant<T1, T2> {
            return A + b;
        }, a);
    } else if constexpr (std::is_same_v<std::variant<T1, T2>, decay_U>) {
        return std::visit([](auto &&A, auto &&B) -> std::variant<T1, T2> {
            return A + B;
        }, a, b);
    } else {
        std::cout << cpp_type_name<std::variant<T1, T2>>() << std::endl;
        std::cout << cpp_type_name<U>() << std::endl;
        throw std::bad_variant_access{};
    }
    
}

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    std::visit([&os](auto &&val) {
        os << val;
    }, a);
    return os;
}

int main() {
    std::vector<int> a = {1, 4, 2, 8, 5, 7};
    std::cout << a << std::endl;
    std::vector<double> b = {3.14, 2.718, 0.618};
    std::cout << b << std::endl;
    auto c = a + b;

    // 应该输出 1
    std::cout << std::is_same_v<decltype(c), std::vector<double>> << std::endl;

    // 应该输出 {4.14, 6.718, 2.618}
    std::cout << c << std::endl;

    std::variant<std::vector<int>, std::vector<double>> d = c;
    std::variant<std::vector<int>, std::vector<double>> e = a;
    d = d + c + e;

    // 应该输出 {9.28, 17.436, 7.236}
    std::cout << d << std::endl;

    return 0;
}
