#include <iostream>
#include <vector>
#include <variant>
#include <utility>

// 请修复这个函数的定义：10 分
template<class T>
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
decltype(auto) operator+(std::vector<T1> const& a, std::vector<T2> const& b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    std::vector<decltype(T1{} + T2{}) > out;
    auto x = a.cbegin();
    auto y = b.cbegin();
    for (;x != a.cend() && y != b.cend(); ++x, ++y)
        out.emplace_back(*x + *y);
    return out;
}

template <class T1, class T2>
decltype(auto) operator+(std::variant<T1, T2> const& a, std::variant<T1, T2> const& b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<T1, T2> out;
    if (std::holds_alternative<T1>(a) && std::holds_alternative<T1>(b))
        out = std::get<T1>(a) + std::get<T1>(b);
    else if (std::holds_alternative<T1>(a) && std::holds_alternative<T2>(b))
        out = std::get<T1>(a) + std::get<T2>(b);
    else if (std::holds_alternative<T2>(a) && std::holds_alternative<T2>(b))
        out = std::get<T2>(a) + std::get<T2>(b);
    else if (std::holds_alternative<T2>(a) && std::holds_alternative<T1>(b))
        out = std::get<T2>(a) + std::get<T1>(b);
    return out;
}

template<class O, class V, class R, size_t N>
constexpr void add_v(O& o, V const& a, R const& b) {
    try
    {
        if (N == a.index())
            o = std::get<N>(a) + b;
    }
    catch (std::bad_variant_access const& ex)
    {
        std::cout << ex.what();
    }
}

template<class O, class V, class R, size_t...N>
decltype(auto) unpack_n(O& o, V const& a, R const& b, std::index_sequence<N...>) {
    static_cast<void>(std::initializer_list<int>{(add_v<O, V, R, N>(o, a, b), 0)...});
}

template <class... Args, class T>
decltype(auto) operator+(std::variant<Args...> const& a, std::vector<T> const& b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<Args...> out;
    unpack_n(out, a, b, std::make_index_sequence<sizeof...(Args)>{});
    return out;
}

template<class V, size_t N>
constexpr decltype(auto) print_v(std::ostream& os, V const& a) {
    try
    {
        if (N == a.index())
            os << std::get<N>(a);
    }
    catch (std::bad_variant_access const& ex)
    {
        std::cout << ex.what();
    }
}

template<class V, size_t...N>
std::ostream& unpack_n(std::ostream& os, V const& a, std::index_sequence<N...>) {
    static_cast<void>(std::initializer_list<int>{(print_v<V, N>(os, a), 0)...});
    return os;
}

template <class... Args>
    requires (sizeof...(Args) > 0)  // no std::variant<>
std::ostream &operator<<(std::ostream &os, std::variant<Args...> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
    return unpack_n(os, a, std::make_index_sequence<sizeof...(Args)>{});
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
