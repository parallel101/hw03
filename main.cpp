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
    std::vector<decltype(T1() + T2())> c(std::min(a.size(), b.size()));
    for (int i = 0; i < c.size(); i++) c[i] = a[i] + b[i];
    return c;
}

template <class T, class... V>
auto operator+(std::variant<V...> const &a, T const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    return std::visit([&](auto const &x) {
        return x + b; }, a);
}

template <class T, class... V>
auto operator+(T const &a, std::variant<V...> const &b) {
    return std::visit([&](auto const &x) {
        return a + x; }, b);
}

template <class R, class... T>
std::ostream &operator<<(std::ostream &os, std::variant<R, T...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    return std::visit([&](auto const &x) -> std::ostream & {
        os << x;
        return os;
    }, a);
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

    std::variant<std::vector<int>, std::vector<double>, std::vector<float>> d = c;
    std::variant<std::vector<int>, std::vector<double>> e = a;
    d = d + c + e;

    // 应该输出 {9.28, 17.436, 7.236}
    std::cout << d << std::endl;

    return 0;
}
