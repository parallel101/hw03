#include <iostream>
#include <vector>
#include <variant>

// 请修复这个函数的定义：10 分
template <typename T>
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
template <class T1, class T2, typename T0 = decltype(std::declval<T1>() + std::declval<T2>())>
std::vector<T0> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    auto size{ std::min(a.size(), b.size()) };
    std::vector<T0> c(size);
    for (std::size_t i{ }; i < size; ++i)
    {
        c[i] = a[i] + b[i];
    }
    return c; 
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return std::visit(
        [](auto &&lhs, auto &&rhs) -> std::variant<T1, T2>
        {
            return lhs + rhs;
        }, 
        a, b
    );
}

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    std::visit(
        [&os](auto &&rhs)
        {
            os << rhs;
        }, 
        a
    );
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
    d = d + static_cast<decltype(d + e)>(c) + e;

    // 应该输出 {9.28, 17.436, 7.236}
    std::cout << d << std::endl;

    return 0;
}
