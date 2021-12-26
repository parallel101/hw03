#include <iostream>
#include <vector>
#include <variant>

// 请修复这个函数的定义：10 分
template <class T>
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
auto operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    /* maybe we should check a.size() == b.size() */
    using T0 = decltype(T1{} + T2{});
    std::vector<T0> results;
    results.reserve(std::min(a.size(), b.size()));
    for (size_t i = 0; i < std::min(a.size(), b.size()); ++i) {
        results.push_back(a[i] + b[i]);
    }
    return results;
}

template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<T1, T2> result;
    std::visit([&] (auto const &v1, auto const &v2) {
        result = v1 + v2;
    }, a, b);
    return result;
}

/* overload operator+ for std::variant and some other non-variant type */
template <class T1, class T2, class T3>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, T3 const &b) {
    using T13 = decltype(T1{} + T3{});
    using T23 = decltype(T2{} + T3{});
    // any elegant way to do this check ?
    static_assert(std::is_same<T13, T1>::value || std::is_same<T13, T2>::value
        || std::is_same<T23, T1>::value || std::is_same<T23, T2>::value);
    std::variant<T1, T2> result;
    std::visit([&] (auto const &v) { result = v + b; }, a);
    return result;
}

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    std::visit([&] (auto const &v) { os << v; }, a);
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
